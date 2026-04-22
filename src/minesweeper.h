#ifndef _SUDOKU_MINESWEEPER_H_
#define _SUDOKU_MINESWEEPER_H_

#include <vector>
#include "common.h"
#include "color.h"

// 扫雷格子的状态
enum class MineState {
    HIDDEN,     // 未翻开
    REVEALED,   // 已翻开
    FLAGGED,    // 已标记
    EXPLODED    // 爆炸
};

// 扫雷格子的内容
struct MineCell {
    bool is_mine;           // 是否是地雷
    int adjacent_mines;     // 周围地雷数量
    MineState state;        // 格子状态
};

class CMinesweeper {
public:
    CMinesweeper(int width = 9, int height = 9, int mines = 10);
    ~CMinesweeper();

    void setMode(KeyMode mode);  // 设置按键模式
    void generate();        // 生成地雷
    void play();           // 游戏主循环
    void show() const;     // 显示游戏界面
    bool reveal(int x, int y);  // 揭示一个格子
    void toggleFlag(int x, int y);  // 切换旗帜标记
    bool isGameOver() const;    // 检查游戏是否结束
    bool isWin() const;         // 检查是否获胜
    bool save(const char* filename);  // 保存游戏
    bool load(const char* filename);  // 加载游戏

private:
    void init();           // 初始化游戏
    void revealCell(int x, int y);  // 揭示格子的具体实现
    int countAdjacentMines(int x, int y) const;  // 计算周围地雷数
    bool isValid(int x, int y) const;  // 检查坐标是否有效
    void revealAllMines();     // 游戏结束时显示所有地雷

private:
    int _width;            // 游戏区域宽度
    int _height;           // 游戏区域高度
    int _total_mines;      // 地雷总数
    int _remaining_cells;  // 剩余未揭示的安全格子
    point_t _cur_point;    // 当前光标位置
    std::vector<std::vector<MineCell>> _board;  // 游戏板
    bool _first_move;      // 是否是第一次点击
    bool _game_over;       // 游戏是否结束
    KeyMap* keyMap;        // 按键映射
};

#endif 