#include <iostream>
#include <sstream>
#include <string>
#include "common.h"
#include "utility.inl"

// return number of grids to be erased
int inputDifficulty()
{
    cls();

    std::string cmd;
    int need_erase_grids = 0;
    while (true)
    {
        std::cout << "设置难度：1简单 2普通 3困难" << std::endl;

        std::cin >> cmd;

        try
        {
            Difficulty difficulty = static_cast<Difficulty>(std::stoi(cmd));
            switch (difficulty)
            {
            case Difficulty::EASY:
                need_erase_grids = 20;
                break;
            case Difficulty::NORMAL:
                need_erase_grids = 35;
                break;
            case Difficulty::HARD:
                need_erase_grids = 50;
                break;
            }
        }
        catch(...) 
        {
            need_erase_grids = 0;
        }

        if (need_erase_grids > 0)
            break;

        std::cout << "输入错误！" << std::endl;
    }

    return need_erase_grids;
}

KeyMode inputKeyMode()
{
    std::string mode;
    do
    {
        message("设置按键模式：1正常 2VIM");

        std::cin >> mode;

        try
        {
            KeyMode kmd = static_cast<KeyMode>(std::stoi(mode));
            return kmd;
        } catch (...)
        {}
        
        message("输入错误！");

    } while (true);
}