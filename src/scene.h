#ifndef _SUDOKU_SCENE_H_
#define _SUDOKU_SCENE_H_

#include <iostream>
#include <vector>
#include "common.h"
#include "block.h"
#include "command.h"

//数独场景类
class CScene
{
  public:
    CScene(int index = 3);
    virtual ~CScene();

    void generate();
    void show() const;              //确保函数只用于“读取”数据，而不“写入”或改变对象

    //将参数声明为 const 可以防止你在函数体中无意间改变这个局部副本
    bool setCurValue(const int nCurValue, int& nLastValue);
    bool setPointValue(const point_t&, const int);
    point_t getCurPoint();

    void eraseRandomGrids(const int count);
    bool isComplete();

    void play();
    bool save(const char *filename);
    bool load(const char *filename);

    void setMode(KeyMode mode);

  private:
    void init(); // 将每个格子的指针放到block里面
    void setValue(const int);
    void setValue(const point_t &, const int);
    void printUnderline(int line_no = -1) const;

private:
    KeyMap *keyMap{};
    int _max_column;
    int subBlockSize;  // 子块的边长
    point_t _cur_point;
    std::vector<CBlock> _column_block;
    std::vector<CBlock> _row_block;
    std::vector<std::vector<CBlock>> _xy_block;
    std::vector<point_value_t> _map;


    std::vector<CCommand> _vCommand;
};

#endif