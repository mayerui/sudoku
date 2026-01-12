#include "scene.h"
#include "command.h"

// 基础构造函数
CCommand::CCommand(CScene *pOwner) : _pOwner(pOwner)
{}

// 完整构造函数
CCommand::CCommand(CScene *pOwner, const point_t &point, int preValue, int curValue)
    : _pOwner(pOwner), _stPoint(point), _nPreValue(preValue), _nCurValue(curValue) {}

// 拷贝构造函数
CCommand::CCommand(const CCommand &rhs)
: _pOwner(rhs._pOwner)
, _stPoint(rhs._stPoint)
, _nPreValue(rhs._nPreValue)
, _nCurValue(rhs._nCurValue)
{}

CCommand::~CCommand(){}

// 执行命令
bool CCommand::execute(int nInputValue)
{
    // 空指针检查：避免调用nullptr的成员函数导致程序崩溃
    if (!_pOwner)
        return false;
    // 记录本次操作的格子位置：获取CScene当前的光标位置
    _stPoint = _pOwner->getCurPoint();
    // 委托CScene执行数值修改：
    // 1. nInputValue：要设置的新值；
    // 2. _nPreValue：输出参数，接收修改前的数值（用于后续undo）；
    // 3. 返回值：是否设置成功（比如数字重复则返回false）
    return _pOwner->setCurValue(nInputValue, _nPreValue);
}

//撤销命令
void CCommand::undo()
{   
    //空指针检查：避免崩溃
    if (_pOwner)
    {
        _pOwner->setPointValue(_stPoint, _nPreValue);
    }

    return;
}