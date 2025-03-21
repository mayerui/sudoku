#ifndef _SUDOKU_COMMAND_H_
#define _SUDOKU_COMMAND_H_

#include <memory>
#include "common.h"

class CScene;
class CCommand
{
public:
    CCommand(CScene* pOwner);
    CCommand(CScene *pOwner, const point_t &point, int preValue);
    CCommand(const CCommand &);
    ~CCommand();

    bool execute(int nInputValue);
    void undo();
   const point_t getPoint() const { return _stPoint; }
   const int getPreValue() const { return _nPreValue; }
    void setPoint(const point_t &point) { _stPoint = point; }
    void setPreValue(int preValue) { _nPreValue = preValue; }

private:
    CScene* _pOwner;
    point_t _stPoint;
    int _nPreValue;
};

#endif