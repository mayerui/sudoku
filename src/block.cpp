#include <cassert>
#include <iostream>
#include "common.h"
#include "block.h"

CBlock::CBlock()
    : _count(0) {}

bool CBlock::isValid() const
{
    assert(MAX_COUNT == _count);

    for (int i = 0; i < _count - 1; ++i)
    {
        for (int j = i + 1; j < _count; ++j)
        {
            if (UNSELECTED == *(_numbers[i]) || UNSELECTED == *(_numbers[j]))
                continue;

            if (*(_numbers[i]) == *(_numbers[j]))
                return false;
        }
    }

    return true;
}

bool CBlock::isFull() const
{
    for (int i = 0; i < _count; ++i)
    {
        int *p_number = _numbers[i];
        if (nullptr == p_number || UNSELECTED == *p_number)
            return false;
    }
    return true;
}

void CBlock::print() const
{
    std::cout << "| ";
    for (int i = 0; i < _count; ++i)
    {
        auto number = *(_numbers[i]);
        if (0 == number)
            std::cout << ' ' << " | ";
        else
            std::cout << number << " | ";
    }
    std::cout << std::endl;
}

void CBlock::push_back(int *point)
{
    assert(nullptr != point);
    _numbers[_count++] = point;
}