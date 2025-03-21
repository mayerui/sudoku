#include "command.h"
#include "scene.h"

CCommand::CCommand(CScene *pOwner) : _pOwner(pOwner) {}

CCommand::CCommand(CScene *pOwner, const point_t &point, int preValue)
    : _pOwner(pOwner),
      _stPoint(point),
      _nPreValue(preValue)
      {}

CCommand::CCommand(const CCommand &rhs)
    : _pOwner(rhs._pOwner),
      _stPoint(rhs._stPoint),
      _nPreValue(rhs._nPreValue) {}

CCommand::~CCommand() {}

bool CCommand::execute(int nInputValue) {
  if (!_pOwner) return false;
  _stPoint = _pOwner->getCurPoint();
  int oldValue = _pOwner->getCurrentValue();
  if (_pOwner->setCurValue(nInputValue)) {
    _nPreValue = oldValue;
    return true;
  }
  return false;
}

void CCommand::undo() {
  if (_pOwner) {
    _pOwner->setPointValue(_stPoint, _nPreValue);
  }

  return;
}