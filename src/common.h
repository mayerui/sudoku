#ifndef _SUDOKU_COMMON_H_
#define _SUDOKU_COMMON_H_

static const unsigned int UNSELECTED = 0;  // 全局静态变量

// 枚举，难度类型
// 强类型枚举，不会隐式转换为int
enum class Difficulty : int
{
    EASY = 1,
    NORMAL,  // 自动递增，值为2
    HARD  //3
};

// 枚举，状态
enum class State : int
{
    INITED = 0,  // 初始状态，不能修改
    ERASED,  // 被擦除状态，可以修改
};

// 枚举，按键模式
enum class KeyMode : int
{
    NORMAL = 1,
    VIM
};

// 键位映射结构体
struct KeyMap
{
    const char ESC = 0x1B;
    const char U = 0x75;
    char UP;
    char LEFT;
    char DOWN;
    char RIGHT;
    const char ENTER = 0x0D;
};

// 普通模式键位映射，继承自 KeyMap
struct Normal : KeyMap
{
    Normal()
    {
        UP = 0x77;
        LEFT = 0x61;
        DOWN = 0x73;
        RIGHT = 0x64;
    }
};

// Vim 模式键位映射，继承自 KeyMap
struct Vim : KeyMap {
    Vim()
    {
        UP = 0x6B;
        LEFT = 0x68;
        DOWN = 0x6A;
        RIGHT = 0x6C;
    }
};

// 点坐标结构体
using point_t = struct point_t {
    int x;
    int y;
};

// 点状态结构体，包含值和状态
using point_value_t = struct point_value_t {
    int value;
    State state;
};

// 点坐标比较类，用于排序或比较
class CPointSort
{
  public:
    bool operator()(const point_t &lhs, const point_t &rhs) const
    {
        // 比较点的坐标是否不同
        if ((lhs.x == rhs.x) && (lhs.y == rhs.y))
            return false;
        else
            return true;
    }
};

#endif