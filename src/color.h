#ifndef _SUDOKU_COLOR_H
#define _SUDOKU_COLOR_H

#include <iostream>
/*
负责终端彩色输出的核心头文件
*/

// 颜色命名空间
namespace Color
{
enum Code
{
    // 样式类：控制文字样式
    BOLD = 1,  // 加粗
    RESET = 0,  // 重置所有样式/颜色

    // 背景色
    BG_BLACK = 40,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_YELLOW = 43,
    BG_BLUE = 44,
    BG_MAGENTA = 45,
    BG_CYAN = 46,
    BG_LIGHT_GRAY = 47, // 实际是白色，终端为正常显示会进行调整
    BG_DEFAULT = 49,
    
    //前景色
    FG_BLACK = 30,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_MAGENTA = 35,
    FG_CYAN = 36,
    FG_LIGHT_GRAY = 37,
    FG_DEFAULT = 39,
    FG_DARK_GRAY = 90,
    FG_LIGHT_RED = 91,
    FG_LIGHT_GREEN = 92,
    FG_LIGHT_YELLOW = 93,
    FG_LIGHT_BLUE = 94,
    FG_LIGHT_MAGENTA = 95,
    FG_LIGHT_CYAN = 96,
    FG_WHITE = 97,
    
};

class Modifier
{
    // 私有成员变量，存储样式、背景色和前景色代码
    Code md, bg, fg;

  public:
  //无参构造函数：默认重置所有样式和颜色
    Modifier() : md(Color::RESET),bg(Color::BG_DEFAULT),fg(Color::FG_DEFAULT) {}
    //有参构造函数：根据传入的样式、背景色和前景色代码进行初始化
    Modifier(Code Mode, Code BackGround, Code FrontGround) : md(Mode),bg(BackGround),fg(FrontGround) {}
    
    // 友元运算符重载：让Modifier对象可以直接输出到输出流中
    friend std::ostream &
    operator<<(std::ostream &os, const Modifier &mod)
    {   
        // 拼接ANSI转义序列，并输出
        return os << "\033[" << mod.md << ";" << mod.bg << ";" << mod.fg << "m";
    }
};
} // namespace Color

#endif