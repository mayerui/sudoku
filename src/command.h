#ifndef _SUDOKU_COMMAND_H_
#define _SUDOKU_COMMAND_H_

#include <memory>
#include "common.h"

/*
将用户的单次格子操作封装为一个 命令对象 CCommand ，包含：
记录操作的上下文（哪个格子、修改前的值、修改后的值）；
提供execute()执行该操作、undo()撤销该操作的接口；
通过指向CScene的指针，直接操作数独场景的底层数据；
所有命令对象会被CScene的_vCommand向量存储，实现 “撤销 / 重做” 的核心逻辑。
*/

// 前向声明
class CScene;
class CCommand
{
public:
    // 构造函数，仅关联所属的CScene对象
    CCommand(CScene* pOwner);
    // 构造函数，初始化命令的具体内容,包括所属CScene对象、操作的格子位置、修改前后的值
    CCommand(CScene *pOwner, const point_t &point, int preValue, int curValue);
    // 拷贝构造函数，至此命令对象的拷贝
    CCommand(const CCommand &);
    // 析构函数
    ~CCommand();

    // 执行命令：将指定格子设置为输入值，返回是否执行成功
    bool execute(int nInputValue);
    // 撤销命令：将指定格子恢复为修改前的值
    void undo();

    // 访问器：获取命令和核心属性
    point_t getPoint() { return _stPoint; }
    int getPreValue() { return _nPreValue; }
    int getCurValue() { return _nCurValue; }
    // 修改器：设置命令和核心属性
    void setPoint(const point_t &point) { _stPoint = point; }
    void setPreValue(int preValue) { _nPreValue = preValue; }
    void setCurValue(int curValue) { _nCurValue = curValue; }

private:
    CScene* _pOwner;  // 指向所属的CScene对象（操作数独场景的入口）
    point_t _stPoint;  // 命令对应的格子坐标
    int _nPreValue;  // 修改前的数值
    int _nCurValue;  // actually the member is never used
};

#endif