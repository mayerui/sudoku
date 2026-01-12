#ifndef _SUDOKU_SCENE_H_
#define _SUDOKU_SCENE_H_

#include <iostream>
#include <vector>

#include "block.h"
#include "command.h"
#include "common.h"

/*
CScene类是数独游戏的总控类，核心职责是：
管理数独完整棋盘（9x9=81 个格子）的底层数据（_map数组）；
按 “行 / 列 / 3x3 宫” 三个维度封装CBlock区块，实现数独规则校验；
处理用户交互（光标移动、数值填充、按键模式切换）；
提供棋盘生成、随机擦除（难度控制）、完成校验、保存 / 加载等游戏核心逻辑；
记录操作命令（支持撤销 / 重做）。
*/
// 数独场景类
class CScene {
 public:
  CScene(int index = 3);  // 构造函数，index表示难度级别
  virtual ~CScene();      // 虚析构函数

  void generate();    // 生成合法的初始数独棋盘
  void show() const;  // 显示完整的数独棋盘

  // 设置当前光标位置的数值，nLastValue返回修改前的数值用于撤销操作
  bool setCurValue(const int nCurValue, int &nLastValue);
  // 设置指定位置的数值
  bool setPointValue(const point_t &, const int);
  point_t getCurPoint();  // 获取当前光标位置

  void eraseRandomGrids(
      const int count);  // 随机擦除指定数量的格子，增加游戏难度
  bool isComplete();     // 检查数独棋盘是否已完整且符合规则

  void play();                      // 主游戏循环，处理用户输入和交互
  bool save(const char *filename);  // 保存当前数独棋盘到文件
  bool load(const char *filename);  // 从文件加载数独棋盘

  void setMode(KeyMode mode);  // 设置按键模式（普通 / VIM）

  // GUI Accessors and Actions
  int getValue(int x, int y) const;
  State getState(int x, int y) const;
  bool undo();  // Try to undo last action

 private:
  void init();                                  // 将每个格子的指针放到block里面
  void setValue(const int);                     // 重载：设置当前光标位置的数值
  void setValue(const point_t &, const int);    // 重载：设置指定位置的数值
  void printUnderline(int line_no = -1) const;  // 打印棋盘下划线

 private:
  KeyMap *keyMap{};         // 按键映射指针
  int _max_column;          // 最大列数（9）
  point_t _cur_point;       // 当前选中的格子坐标
  CBlock _column_block[9];  // 列区块数组，9列，每列一个CBlock
  CBlock _row_block[9];     // 行区块数组，9行，每行一个CBlock
  CBlock _xy_block[3][3];   // 3x3宫区块数组，3行3列共9个CBlock
  point_value_t _map[81];   // 数独棋盘数据，81个格子

  std::vector<CCommand> _vCommand;  // 命令记录表（用于撤销/重做）
};

#endif