#include "block.h"

#include <cassert>
#include <iostream>
#include <string>

#include "color.h"
#include "common.h"
#include "display_symbol.h"

// 构造函数
CBlock::CBlock()
    : _count(0) {}

// 检验区块合法性
bool CBlock::isValid() const
{
    // 断言：区块必须填满9个格子才能进行校验合法性
    assert(MAX_COUNT == _count);

    for (int i = 0; i < _count - 1; ++i)
    {
        for (int j = i + 1; j < _count; ++j)
        {
            // 跳过为填充的格子（值为UNSELECTED=0）,不参与重复检验
            if (UNSELECTED == _numbers[i]->value || UNSELECTED == _numbers[j]->value)
                continue;
            
            // 两个非0数字重复，区块不合法
            if (_numbers[i]->value == _numbers[j]->value)
                return false;
        }
    }

    return true;
}

// 检验区块是否填满
bool CBlock::isFull() const
{
    for (int i = 0; i < _count; ++i)
    {
        point_value_t *p_point_value = _numbers[i];
        // 空指针或数值为0都表示未填满
        if (nullptr == p_point_value || UNSELECTED == p_point_value->value)
            return false;
    }
    return true;
}

// 打印区块内容
void CBlock::print(int cur_point, int highlighted_num) const {
    // 打印区块起始边框：红色加粗的PIPE符（|）
  std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) << PIPE << Color::Modifier()<< " ";
  for (int i = 0; i < _count; ++i) {
    // 解引用指针，获取格子的数值和状态
    auto number = *(_numbers[i]);
    Color::Code pipe_color, num_bgcolor, num_fgcolor;
    std::string num = " ";  //默认空格，对应数值0的情况
    
    // 每三个数字后面的分隔符PIPE为红色，其余为默认颜色
    if ((i+1)%3 == 0) pipe_color = Color::FG_RED;
    else pipe_color = Color::FG_DEFAULT;

    if (i==cur_point) num_bgcolor = Color::BG_BLUE;
    else if (highlighted_num != UNSELECTED && number.value == highlighted_num)
        num_bgcolor = Color::BG_YELLOW; // 与当前光标相同数字高亮为黄色
    else num_bgcolor = Color::BG_DEFAULT;
    
    if (number.state == State::ERASED) num_fgcolor = Color::FG_GREEN;
    else num_fgcolor = Color::FG_DEFAULT;

    if (number.value != 0) {
      num = std::to_string(number.value);
    }

    std::cout << Color::Modifier(Color::BOLD, num_bgcolor, num_fgcolor) << num
              << Color::Modifier() << " " 
              << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, pipe_color) << PIPE 
              << Color::Modifier() << " ";
  }
  std::cout << std::endl;
}

// 添加格子到区块
void CBlock::push_back(point_value_t *point)
{
    // 断言：传入的指针不能为空
    assert(nullptr != point);
    _numbers[_count++] = point;  // 存入数组然后_count自增
}

//获取指定索引的格子数值
int CBlock::getNumberValue(int index) const 
{
    // 索引合法，返回对应数值
    if (index >= 0 && index < _count) {
        return _numbers[index]->value;
    }
    return UNSELECTED; // 或抛异常
}