#include "scene.h"
#include "command.h"

CCommand::CCommand(CScene *pOwner) : _pOwner(pOwner)
{}

CCommand::CCommand(const CCommand &rhs)
: _pOwner(rhs._pOwner.get())
, _stPoint(rhs._stPoint)
, _nPreValue(rhs._nPreValue)
, _nCurValue(rhs._nCurValue)
{}

CCommand::~CCommand(){}

bool CCommand::excute(int nInputValue)
{
    if (!_pOwner)
        return false;

    _stPoint = _pOwner.get()->getCurPoint();
    return _pOwner.get()->setCurValue(nInputValue, _nPreValue);
}

void CCommand::undo()
{
    if (_pOwner)
    {
        _pOwner.get()->setPointValue(_stPoint, _nPreValue);
    }

    return;
}