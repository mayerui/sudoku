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

void CBlock::print(int cur_point) const {
  std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) << PIPE << Color::Modifier()<< " ";
  for (int i = 0; i < _count; ++i) {
    auto number = *(_numbers[i]);
    Color::Code pipe_color, num_bgcolor, num_fgcolor;
    char num;
    
    if ((i+1)%3 == 0) pipe_color = Color::FG_RED;
    else pipe_color = Color::FG_DEFAULT;
    if (i==cur_point) num_bgcolor = Color::BG_BLUE;
    else num_bgcolor = Color::BG_DEFAULT;
    if (number.state == State::ERASED) num_fgcolor = Color::FG_GREEN;
    else num_fgcolor = Color::FG_DEFAULT;
    if (number.value == 0) num = ' ';
    else num = '0' + number.value;

    std::cout << Color::Modifier(Color::BOLD, num_bgcolor, num_fgcolor) << num
              << Color::Modifier() << " " 
              << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, pipe_color) << PIPE 
              << Color::Modifier() << " ";
  }
  std::cout << std::endl;
}

void CBlock::push_back(point_value_t *point)
{
    assert(nullptr != point);
    _numbers[_count++] = point;
}
