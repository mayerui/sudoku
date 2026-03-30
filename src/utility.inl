#ifndef _SUDOKU_UTILITY_INL_
#define _SUDOKU_UTILITY_INL_

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>
#include <numeric>

//not real random,return number between [begin,end]
//内联函数
inline unsigned int random(int begin, int end)
{
    std::random_device rd;
    std::mt19937 g(rd());                       //Mersenne Twister 是一种常用的伪随机数生成算法
    //创建一个均匀分布的整数分布对象，其范围为[begin, end]——std::uniform_int_distribution<unsigned int>(begin, end)
    return std::uniform_int_distribution<unsigned int>(begin, end)(g);
}

inline std::vector<int> get_unit(int max_value){
    std::vector<int> result(max_value);  // 创建大小为 max_value 的 vector
    std::iota(result.begin(), result.end(), 1);  // 从1开始填充直到 max_value
    return result;
}

inline std::vector<int> shuffle_unit(int max_value){
    std::vector<int> unit = get_unit(max_value);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(unit.begin(), unit.end(), g);
    return unit;
}

inline void message(const char* msg = "", bool lf = true)
{
    std::cout << msg;
    if (lf) std::cout << std::endl;
}

inline void message(const std::string& msg, bool lf = true) {
  message(msg.c_str(), lf);
}

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

//在不同操作系统上执行清屏操作
inline void cls(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

#endif
