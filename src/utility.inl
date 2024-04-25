#ifndef _SUDOKU_UTILITY_INL_
#define _SUDOKU_UTILITY_INL_

#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>

//not real random,return number between [begin,end]
inline unsigned int random(int begin, int end)
{
    assert(end >= begin && begin >= 0);
    srand(time(NULL));
    return (unsigned int)rand() % (end - begin + 1) + begin;
}

//网上找的均匀化随机数算法,不含max,非随机，弃用
inline int AverageRandom(int min, int max)
{
    int minInteger = min * 10000;
    int maxInteger = max * 10000;
    srand(time(NULL));
    int randInteger = rand() * rand();
    int diffInteger = maxInteger - minInteger;
    int resultInteger = randInteger % diffInteger + minInteger;

    return (resultInteger / 10000);
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
    #include <fcntl.h>  /* 使用fcntl */
    #elif __APPLE__
    #include <termios.h>
    #endif
    inline char getch(void)
    {
        struct termios tmtemp, tm;
        // char c;
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
#endif

inline void cls(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

#endif
