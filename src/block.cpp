#include "block.h"

#include <cassert>
#include <iostream>

#include "color.h"
#include "common.h"
#include "display_symbol.h"

// 初始化列表的方式构造函数
CBlock::CBlock()
    : _count(0) {}

// 函数后面带const 相当于“只读函数”，不会改变成员变量
bool CBlock::isValid() const
{
    assert(MAX_COUNT == _count);

    for (int i = 0; i < _count - 1; ++i)
    {
        for (int j = i + 1; j < _count; ++j)
        {   
            // 如果等于该值为‘0’跳过
            if (UNSELECTED == _numbers[i]->value || UNSELECTED == _numbers[j]->value)
                continue;
            // 9宫格有重复，无效
            if (_numbers[i]->value == _numbers[j]->value)
                return false;
        }
    }

    return true;
}
// 判断9宫格是否填满（不能有‘0’）
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

// 打印每个block，注意这里是每个行
void CBlock::print() const {
  std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) << PIPE << Color::Modifier()<< " ";
  // std::cout << PIPE << " ";
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

void CBlock::push_back(point_value_t *point)
{
    assert(nullptr != point);
    _numbers[_count++] = point;
}
