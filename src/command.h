#ifndef _SUDOKU_COMMAND_H_
#define _SUDOKU_COMMAND_H_

#include <memory>
#include "common.h"

class CScene;
class CCommand
{
public:
    CCommand(CScene* pOwner);
    CCommand(const CCommand &);
    ~CCommand();

    bool excute(int nInputValue);
    void undo();

private:
    std::unique_ptr<CScene> _pOwner;
    point_t _stPoint;
    int _nPreValue;
    int _nCurValue;
};

#endif