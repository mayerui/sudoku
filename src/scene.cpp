#include <cmath>
#include <iostream>
#include <memory.h>
#include <map>
#include "common.h"
#include "scene.h"
#include "utility.inl"

CScene::CScene(int index)
    : _index(index),
      _max_column(pow(index, 2)),
      _cur_point({0, 0})
{
    init();
}

CScene::~CScene()
{
    //delete _map;
}

void CScene::show() const
{
    system("cls");

    printUnderline();

    for (int row = 0; row < _max_column; ++row)
    {
        auto block = _row_block[row];
        block.print();
        printUnderline(row);
    }
}

void CScene::printUnderline(int line_no) const
{
    std::string underline;

    for (int colunm = 0; colunm < 9; ++colunm)
    {
        if (_cur_point.y == line_no && _cur_point.x == colunm)

            underline += "--^-";
        else
            underline += "----";
    }
    underline += '-';

    std::cout << underline.c_str() << std::endl;
}

void CScene::init()
{
    memset(_map, UNSELECTED, sizeof _map);

    //column_block 所有列
    for (int column = 0; column < _max_column; ++column)
    {
        CBlock column_block;

        for (int row = 0; row < _max_column; ++row)
        {
            column_block.push_back(_map + row * 9 + column);
        }
        _column_block[column] = column_block;
    }

    //row_block 所有行
    for (int row = 0; row < _max_column; ++row)
    {
        CBlock row_block;

        for (int column = 0; column < _max_column; ++column)
        {
            row_block.push_back(_map + row * 9 + column);
        }
        _row_block[row] = row_block;
    }

    //xy_block 所有九宫格, [行][列]
    for (int block_index = 0; block_index < _max_column; ++block_index)
    {
        CBlock xy_block;

        int xy_begin = block_index / 3 * 27 + block_index % 3 * 3;
        xy_block.push_back(_map + xy_begin);
        xy_block.push_back(_map + xy_begin + 1);
        xy_block.push_back(_map + xy_begin + 2);
        xy_block.push_back(_map + xy_begin + 9);
        xy_block.push_back(_map + xy_begin + 10);
        xy_block.push_back(_map + xy_begin + 11);
        xy_block.push_back(_map + xy_begin + 18);
        xy_block.push_back(_map + xy_begin + 19);
        xy_block.push_back(_map + xy_begin + 20);
        _xy_block[block_index / 3][block_index % 3] = xy_block;
    }

    return;
}

bool CScene::setCurValue(const int value)
{
    auto point = _map[_cur_point.x + _cur_point.y * 9];
    if (ERASED == point.state)
    {
        setValue(_cur_point, value);
        return true;
    }
    else
        return false;
}

void CScene::setValue(const point_t p, const int value)
{
    _map[p.x + p.y * 9].value = value;
}

//选择count个格子清空
void CScene::eraseRandomGrids(const int count)
{
    int i = 0;
    while (i < count)
    {
        point_t p = {0};
        p.x = AverageRandom(0, 9);
        p.y = AverageRandom(0, 9);

        auto &point = _map[p.x + p.y * 9];

        if (ERASED == _map[p.x + p.y * 9].state)
            continue;

        setValue(p, 0);
        point.state = ERASED;
        ++i;
    }
}

bool CScene::isComplete()
{
    //任何一个block未被填满，则肯定未完成
    for (size_t i = 0; i < 81; ++i)
    {
        if (UNSELECTED == _map[i].value)
            return false;
    }

    //同时block里的数字还要符合规则
    for (int row = 0; row < 9; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            if (!_row_block[row].isValid() || !_column_block[column].isValid() || !_xy_block[row / 3][column / 3].isValid())
                return false;
        }
    }

    return true;
}

void CScene::play()
{
    show();

    char key = '\0';
    while (1)
    {
        key = getch(); //windows
        if ('0' <= key && '9' >= key)
        {
            if (!setCurValue(key - '0'))
                std::cout << "该数字不允许修改" << std::endl;
            else
                show();
            continue;
        }

        switch (key)
        {
        case 0x1B: //ESC
            std::cout << "quit game." << std::endl;
            system("pause");
            exit(0);
            return;
        case 0x61: //a
            _cur_point.x = (_cur_point.x - 1) < 0 ? 0 : _cur_point.x - 1;
            show();
            break;
        case 0x64: //d
            _cur_point.x = (_cur_point.x + 1) > 8 ? 8 : _cur_point.x + 1;
            show();
            break;
        case 0x73: //s
            _cur_point.y = (_cur_point.y + 1) > 8 ? 8 : _cur_point.y + 1;
            show();
            break;
        case 0x77: //w
            _cur_point.y = (_cur_point.y - 1) < 0 ? 0 : _cur_point.y - 1;
            show();
            break;
        case 0x0D: //enter
            if (isComplete())
            {
                std::cout << "congratulation! you win!" << std::endl;
                return;
            }
            else
            {
                std::cout << "sorry, not completed." << std::endl;
            }
            break;
        default:
            break;
        }
    }
}

//一个场景可以多次被初始化
void CScene::generate()
{
    static char map_pattern[10][10] = {
        "ighcabfde",
        "cabfdeigh",
        "fdeighcab",
        "ghiabcdef",
        "abcdefghi",
        "defghiabc",
        "higbcaefd",
        "bcaefdhig",
        "efdhigbca"};

    //产生字母到数字的随机映射
    std::map<char, int> hash_map;
    for (int i = 1; i <= 9; ++i)
    {
        char key = 'a';
        do
        {
            key = 'a' + AverageRandom(0, 9);
        } while (hash_map.end() != hash_map.find(key));

        hash_map[key] = i;
    }

    //填入场景
    for (int row = 0; row < 9; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            point_t point = {row, column};
            char key = map_pattern[row][column];
            setValue(point, hash_map[key]);
        }
    }

    assert(isComplete());

    return;
}