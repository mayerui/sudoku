#ifndef _SUDOKU_BLOCK_H_
#define _SUDOKU_BLOCK_H_

#include<vector>
#include "common.h"

// 数独行政区域类
class CBlock
{
  public:
    explicit CBlock(int maxCount = 9);
    bool isValid() const;
    bool isFull() const;
    void print(int max_col = 9, int cur_point = -1) const;
    void push_back(point_value_t *point);

  private:
    int _count;       //用于记录当前已有多少个元素存储在 _numbers 数组中。它既代表数组中有效数据的数量，也是下一个新元素要插入的位置。
    //point_value_t *_numbers[MAX_COUNT];
    int _maxCount;      // 动态设置的块的容量
    int subBlockSize;
    std::vector<point_value_t *> _numbers;
};

#endif