#include <iostream>
#include <sstream>
#include <string>
#include "common.h"

//return erase grid number
int inputDifficulty()
{
    std::string cmd;
    while (1)
    {
        std::cout << "设置难度：1简单 2普通 3困难" << std::endl;

        std::cin >> cmd;

        int difficulty = atoi(cmd.c_str());

        switch (difficulty)
        {
        case EASY:
            return 20;
        case NORMAL:
            return 35;
        case HARD:
            return 50;
        default:
            std::cout << "输入错误！" << std::endl;
            continue;
        }
    }

    return 0;
}