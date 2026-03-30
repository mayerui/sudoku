#include <iostream>
#include <string>

#include "common.h"
#include "i18n.h"
#include "utility.inl"

// return number of grids to be erased
int inputDifficulty(int index)
{
    cls();                                         //清屏

    std::string cmd;                        //存储用户输入的字符串
    int need_erase_grids = 0;         //用于存储将被擦除的网格数量
    while (true)
    {
        message(I18n::Instance().Get(I18n::Key::ASK_DIFFICULTY));

        std::cin >> cmd;

        try
        {
            Difficulty difficulty = static_cast<Difficulty>(std::stoi(cmd));

            //按比例计算要删除的格子的数量
            // easy；normal；hard = 24.7%；43.2%；61.7%
            double totalGrids = pow(index, 4);

            switch (difficulty)
            {
            case Difficulty::EASY:
                need_erase_grids = static_cast<int>(totalGrids * 0.247);
                break;
            case Difficulty::NORMAL:
              need_erase_grids = static_cast<int>(totalGrids * 0.432);
                break;
            case Difficulty::HARD:
              need_erase_grids = static_cast<int>(totalGrids * 0.617);
                break;
            }
        }

        catch(...) 
        {
            need_erase_grids = 0;
        }

        if (need_erase_grids > 0)
            break;

        message(I18n::Instance().Get(I18n::Key::INPUT_ERROR));
    }

    return need_erase_grids;
}

//获取用户输入的按键模式，并将其转换为 KeyMode 类型
KeyMode inputKeyMode() {
  std::string mode;
  do {
    message(I18n::Instance().Get(I18n::Key::ASK_KEY_MAP));

    std::cin >> mode;

    try {
      KeyMode kmd = static_cast<KeyMode>(std::stoi(mode));
      return kmd;
    } catch (...) {
    }

    message(I18n::Instance().Get(I18n::Key::INPUT_ERROR));
  } while (true);
}

void InputLanguage() {
  std::string language;
  do {
    message("1English 2中文");
    std::cin >> language;

    try {
      auto l = static_cast<Language>(std::stoul(language) - 1);
      if (l < Language::MAX) {
        I18n::Instance().SetLanguage(l);
        return;
      }
    } catch (...) {
    }

    message(I18n::Instance().Get(I18n::Key::INPUT_ERROR));
  } while (true);
}
