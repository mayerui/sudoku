#ifndef _SUDOKU_COMMAND_H_
#define _SUDOKU_COMMAND_H_

#include <memory>
#include "common.h"

class CScene;
class CCommand
{
public:
    CCommand(CScene* pOwner);
    CCommand(CScene *pOwner, const point_t &point, int preValue, int curValue);
    CCommand(const CCommand &);
    ~CCommand();

    bool execute(int nInputValue);
    void undo();
    point_t getPoint() { return _stPoint; }
    int getPreValue() { return _nPreValue; }
    int getCurValue() { return _nCurValue; }
    void setPoint(const point_t &point) { _stPoint = point; }
    void setPreValue(int preValue) { _nPreValue = preValue; }
    void setCurValue(int curValue) { _nCurValue = curValue; }

private:
    CScene* _pOwner;
    point_t _stPoint;
    int _nPreValue;
    int _nCurValue;  // actually the member is never used
};

#endif