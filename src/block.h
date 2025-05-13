#ifndef _SUDOKU_BLOCK_H_
#define _SUDOKU_BLOCK_H_

#include "common.h"

// 数独行政区域类
class CBlock
{
    static const int MAX_COUNT = 9;

  public:
    CBlock();
    bool isValid() const;
    bool isFull() const;
    void print(int cur_point = -1, char highlighted_num = '0') const;
    void push_back(point_value_t *point);
    char getNumberValue(int index) const;

  private:
    int _count;
    point_value_t *_numbers[MAX_COUNT];
};

#endif