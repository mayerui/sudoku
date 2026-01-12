#include <iostream>
#include <string>

#include "common.h"
#include "i18n.h"
#include "utility.inl"

//获取难度并但返回擦除格子数
// return number of grids to be erased
int inputDifficulty()
{
    // 清屏：提升交互体验，避免终端内容混乱
    cls();

    std::string cmd;
    int need_erase_grids = 0; //要返回的擦除格子数，
    while (true)  //无限循环，直到用户输入有效难度选项
    {
        // 提示用户输入难度选择
        message(I18n::Instance().Get(I18n::Key::ASK_DIFFICULTY));

        // 读取用户输入（字符串，避免直接读int导致导致非法输入崩溃）
        std::cin >> cmd;

        try
        {   
            // 将输入字符串转化为int，强转为Difficulty枚举
            Difficulty difficulty = static_cast<Difficulty>(std::stoi(cmd));
            // 根据难度设置对应的擦除格子数
            switch (difficulty)
            {
            case Difficulty::EASY:
                need_erase_grids = 20;  // 简单难度擦除20个格子
                break;
            case Difficulty::NORMAL:
                need_erase_grids = 35;  // 普通难度擦除35个格子
                break;
            case Difficulty::HARD:
                need_erase_grids = 50;  // 困难难度擦除50个格子
                break;
            }
        }
        catch(...) // 捕获所有异常，防止非法输入导致程序崩溃
        {
            need_erase_grids = 0;  // 异常则置为0，表示非法输入
        }

        if (need_erase_grids > 0)  // 输入有效，跳出循环
            break;
        // 输入无效，提示错误信息，继续循环
        message(I18n::Instance().Get(I18n::Key::INPUT_ERROR));
    }

    return need_erase_grids;
}

// 获取按键模式
KeyMode inputKeyMode() {
  std::string mode;
  do { // do-while循环，至少执行一次
    //打印多语言的按键模式提示
    message(I18n::Instance().Get(I18n::Key::ASK_KEY_MAP));

    std::cin >> mode;  // 读取输入的字符串

    try {
      KeyMode kmd = static_cast<KeyMode>(std::stoi(mode));
      return kmd;
    } catch (...) {  // 捕获所有异常
    }

    // 输入非法，打印错误提示
    message(I18n::Instance().Get(I18n::Key::INPUT_ERROR));
  } while (true);  // 无限循环，直到返回有效KeyMode
}

// 配置游戏语言
void InputLanguage() {
  std::string language;
  do {
    // 打印语言选择提示
    message("1English 2中文");
    std::cin >> language;

    try {
      // 将输入转换为Language枚举
      auto l = static_cast<Language>(std::stoul(language) - 1);
      // 验证语言枚举值是否有效
      if (l < Language::MAX) {
        I18n::Instance().SetLanguage(l);
        return;
      }
    } catch (...) {
    }

    message(I18n::Instance().Get(I18n::Key::INPUT_ERROR));
  } while (true);
}
