#ifndef _SUDOKU_UTILITY_INL_
#define _SUDOKU_UTILITY_INL_

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

/*
数独游戏的核心工具函数内联文件，封装了跨平台的随机数生成、数组打乱、终端交互（打印、清屏、无回显获取字符）
等通用工具函数，是整个项目中被CScene（棋盘生成）、input（用户交互）等模块依赖的基础工具层，
所有函数都用inline修饰以提升执行效率。

生成高质量伪随机数、打乱 1-9 数组（数独棋盘随机生成的核心）；
封装统一的终端打印接口（支持字符串 / 字符指针，可选换行）；
实现跨平台的 “无回显获取单个字符”（用户按键交互的核心）；
提供跨平台清屏函数，保证交互界面整洁。
*/

//生成指定范围的随机数
//not real random,return number between [begin,end]
inline unsigned int random(int begin, int end)
{
    std::random_device rd;  // 随机数种子，部分平台为伪随机
    std::mt19937 g(rd());  //梅森旋转算法，高质量伪随机数生成器
    // 均匀分布：生成范围内的无符号整数
    return std::uniform_int_distribution<unsigned int>(begin, end)(g);
}

// 返回 1-9 的顺序数组
inline std::vector<int> get_unit(){
    return std::vector<int> {1, 2, 3, 4, 5, 6, 7, 8, 9};
}

// 打乱1-9数组顺序
inline std::vector<int> shuffle_unit(){
    std::vector<int> unit = get_unit();
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(unit.begin(), unit.end(), g);  // 打乱数组
    return unit;
}

// 统一的终端打印接口
// 重载1：打印C风格字符串，可选是否换行
inline void message(const char* msg = "", bool lf = true)
{
    std::cout << msg;
    if (lf) std::cout << std::endl;
}
// 重载2：打印C++字符串，可选是否换行
inline void message(const std::string& msg, bool lf = true) {
  message(msg.c_str(), lf);
}

// 跨平台无回显获取单个字符
// Windows平台使用conio.h的_getch()
// Linux和MacOS平台使用termios.h实现类似功能
#ifdef _WIN32
#include <conio.h>
#else
    #ifdef __linux__
    #include <termio.h>
    #include <cstdio>
    #include <fcntl.h>
    #elif __APPLE__
    #include <termios.h>
    #include <fcntl.h>
    #endif
    inline char getch(void)
    {
        struct termios tmtemp, tm;
        int c;
        int fd = 0;
        if (tcgetattr(fd, &tm) != 0)
        { /*获取当前的终端属性设置，并保存到tm结构体中*/
            return -1;
        }
        tmtemp = tm;
        cfmakeraw(&tmtemp); /*将tetemp初始化为终端原始模式的属性设置*/
        if (tcsetattr(fd, TCSANOW, &tmtemp) != 0)
        { /*将终端设置为原始模式的设置*/
            return -1;
        }
        c = getchar();
        if (c == 27)  /* ESC返回27，上下左右为(27,91,xx) 与ESC的27冲突 */
        {
            int back_flags = fcntl(fd, F_GETFL);
            /*将fd设置为非阻塞的，没有输入时可以立即返回*/
            fcntl(fd, F_SETFL, back_flags | O_NONBLOCK);
            c = getchar();
            if (c == EOF)
            {
                c = 27; 
            }
            else
            {
                while ( (c = getchar()) != EOF) 
                {
                }
            }
            fcntl(fd, F_SETFL, back_flags);
        }
        if (tcsetattr(fd, TCSANOW, &tm) != 0)
        { /*接收字符完毕后将终端设置回原来的属性*/
            return 0;
        }
        return static_cast<char>(c);
    }

    inline char _getch() { return getch(); }
#endif

// 跨平台清屏函数
inline void cls(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

#endif
