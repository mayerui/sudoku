#ifndef _SUDOKU_BLOCK_H_
#define _SUDOKU_BLOCK_H_

#include "common.h"

// 数独行政区域类，用于表示数独中的3x3区块
class CBlock
{
    static const int MAX_COUNT = 9;  // 静态常量区：区块最大格子数量

  public:
    CBlock();  // 构造函数
    bool isValid() const;  // 检查区块是否符合数独规则 
    bool isFull() const;  // 检查区块是否填满
    void print(int cur_point = -1, int highlighted_num = '0') const;  // 打印区块内容
    void push_back(point_value_t *point);  // 添加格子到区块
    int getNumberValue(int index) const;  // 获取指定格子的数值

  private:
    int _count;  // 已添加的格子数量
    point_value_t *_numbers[MAX_COUNT];  // 指向区块内格子的指针数组
};

#endif