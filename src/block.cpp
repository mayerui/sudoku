#include "block.h"

#include <cassert>
#include <iostream>

#include "color.h"
#include "common.h"
#include "display_symbol.h"


CBlock::CBlock()
    : _count(0) {}

bool CBlock::isValid() const
{
    assert(MAX_COUNT == _count);

    for (int i = 0; i < _count - 1; ++i)
    {
        for (int j = i + 1; j < _count; ++j)
        {
            if (UNSELECTED == _numbers[i]->value || UNSELECTED == _numbers[j]->value)
                continue;

            if (_numbers[i]->value == _numbers[j]->value)
                return false;
        }
    }

    return true;
}

bool CBlock::isFull() const
{
    for (int i = 0; i < _count; ++i)
    {
        point_value_t *p_point_value = _numbers[i];
        if (nullptr == p_point_value || UNSELECTED == p_point_value->value)
            return false;
    }
    return true;
}
/*
void CBlock::print() const {
  std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) << PIPE << Color::Modifier()<< " ";
  for (int i = 0; i < _count; ++i) {
    auto number = *(_numbers[i]);
    if((i+1)%3 == 0) {
      if (0 == number.value)
        std::cout << ' ' << " " << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) << PIPE << Color::Modifier() << " ";
      else {
        if (number.state == State::ERASED)
          std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_GREEN) << number.value
                    << Color::Modifier() << " " 
                    << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) << PIPE 
                    << Color::Modifier() << " ";
        else
          std::cout << number.value << " " << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) 
                    << PIPE << Color::Modifier() << " ";
      }
    } else {
      if (0 == number.value)
      std::cout << ' ' << " " << PIPE << " ";
      else {
        if (number.state == State::ERASED)
          std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_GREEN) << number.value
                    << Color::Modifier() << " " << PIPE << " ";
        else
          std::cout << number.value << " " << PIPE << " ";
      }
    }
    
  }
  std::cout << std::endl;
}
*/

void CBlock::print(const point_t &cursor,int _row) const {
    // 遍历每个数字，打印当前行的数据，光标所在位置高亮显示。
    std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED)
              << PIPE << Color::Modifier() << " ";
    for (int i = 0; i < _count; ++i) {
        auto number = *(_numbers[i]);

        // 判断当前格子是否是光标位置
        bool isCursor =
            (cursor.y == _row && cursor.x == i);  // 假设 _row 表示当前块的行号

        // 应用背景颜色高亮显示光标位置
        Color::Modifier bgHighlight =
            isCursor ? Color::Modifier(Color::BOLD, Color::BG_BLUE,
                                       Color::FG_DEFAULT)
                     : Color::Modifier(Color::BOLD, Color::BG_DEFAULT,
                                       Color::FG_DEFAULT);
        Color::Modifier defaultHightlight =Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_DEFAULT);
        Color::Code bgLight = isCursor ? Color::BG_BLUE : Color::BG_DEFAULT;
        Color::Code fgLight = isCursor ? Color::BG_BLUE : Color::BG_DEFAULT;

        if ((i + 1) % 3 == 0) {
            if (0 == number.value) {
                std::cout << bgHighlight << ' ';
                std::cout << defaultHightlight
                          << " "
                          << Color::Modifier(Color::BOLD, Color::BG_DEFAULT,
                                             Color::FG_RED)
                          << PIPE << Color::Modifier() << " ";
            } else {
                if (number.state == State::ERASED) {
                  std::cout << bgHighlight
                            << Color::Modifier(Color::BOLD,bgLight,
                                               Color::FG_GREEN)
                            << number.value << Color::Modifier();
                  std::cout << defaultHightlight << " "
                            << Color::Modifier(Color::BOLD, Color::BG_DEFAULT,
                                               Color::FG_RED)
                            << PIPE << Color::Modifier() << " ";
                } else {
                  std::cout << bgHighlight << number.value;
                  std::cout << defaultHightlight << " "
                            << Color::Modifier(Color::BOLD, Color::BG_DEFAULT,
                                               Color::FG_RED)
                            << PIPE << Color::Modifier() << " ";
                }
            }
        } else {
            if (0 == number.value) {
                std::cout << bgHighlight;
                std::cout << Color::Modifier(Color::BOLD, bgLight,
                                             Color::FG_DEFAULT)
                          << ' ' << defaultHightlight << " " << PIPE << " ";
            }
            else {
                if (number.state == State::ERASED) {
                  std::cout << bgHighlight
                            << Color::Modifier(Color::BOLD, bgLight,
                                               Color::FG_GREEN)
                            << number.value;
                  std::cout << defaultHightlight << Color::Modifier() << " "
                            << PIPE << " ";
                } else {
                  std::cout << bgHighlight << number.value;
                  std::cout << defaultHightlight << " " << PIPE << " ";
                }
             }
        }
    }
    std::cout << std::endl;
}

void CBlock::push_back(point_value_t *point)
{
    assert(nullptr != point);
    _numbers[_count++] = point;
}
