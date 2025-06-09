#include "scene.h"

#include <memory.h>

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <thread>
#include<string>


#include "common.h"
#include "display_symbol.h"
#include "i18n.h"
#include "utility.inl"
#include "color.h"

CScene::CScene(int index)
    : _max_column(static_cast<size_t>(pow(index, 2))) 
    , _cur_point({0, 0})
{
    //调整vector的大小
  _column_block.resize(_max_column, CBlock(_max_column));
    _row_block.resize(_max_column, CBlock(_max_column));

    subBlockSize = index;
    //_xy_block.resize(subBlockSize, std::vector<CBlock>(subBlockSize));
    _xy_block.resize(subBlockSize);  // 先调整外层 vector 的大小
    for (size_t i = 0; i < subBlockSize; ++i) {
      _xy_block[i].resize(subBlockSize, CBlock(_max_column));
    }

    _map.resize(_max_column * _max_column);
    init();
}

//谁分配，谁释放
CScene::~CScene()
{
    if(keyMap) delete keyMap;
}

//在控制台上展示整个棋盘（或网格）状态，并且高亮显示当前光标所在的位置
void CScene::show() const
{
    cls();

    printUnderline();               

    for (int row = 0; row < _max_column; ++row)
    {
        CBlock block = _row_block[row];
      if (_cur_point.y == row)
        block.print(_max_column, _cur_point.x);  // x 表示列，y 表示行
      else
        block.print(_max_column);
        printUnderline(row);
    }
}

void CScene::setMode(KeyMode mode)
{
    switch (mode)
    {
    case KeyMode::NORMAL:
        keyMap = new Normal;
        break;

    case KeyMode::VIM:
        keyMap = new Vim;
        break;
    }
}

//用来重复整个字符串 pattern 指定的次数
std::string repeatPattern(const std::string &pattern, int count) {
  std::string result;
  result.reserve(pattern.size() * count);  // 提前分配空间，提升效率
  for (int i = 0; i < count; ++i) {
    result += pattern;
  }
  return result;
}


void CScene::printUnderline(int line_no) const {
  int fieldWidth = std::to_string(_max_column).length();
  int cellWidth = fieldWidth + 2;

  bool isCurLine = (_cur_point.y == line_no);

  for (int col = 0; col < _max_column; ++col) {
    if ((col % subBlockSize) == 0 || line_no == -1 ||
        ((line_no + 1) % subBlockSize == 0)) {
      std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT,
                                   Color::FG_RED)
                << CORNER << Color::Modifier();
    } else {
      std::cout << CORNER;
    }

// 生成当前单元格内部的字符串：重复完整的 LINE 字符，次数为 cellWidth
    std::string cellInterior = repeatPattern(LINE, cellWidth);

    if (isCurLine && _cur_point.x == col) {
      size_t patternLen = std::strlen(LINE);  // 每个 LINE 在 UTF-8 下的字节数
      int index = (cellWidth + 1) / 2;  // 选择逻辑上的中间偏右位置（注意：这里
                                        // index 的定义应与视觉需求协调）
      size_t bytePos = index * patternLen;  // 计算对应的字节起始位置
      // 替换从 bytePos 开始的 patternLen 个字节为箭头字符串 "^"
      cellInterior.replace(bytePos, patternLen, "^");
    }


    // 判断是否需要对这一单元格的内容也进行边界样式（如子块边界使用红色粗体）
    if (line_no == -1 || ((line_no + 1) % subBlockSize == 0)) {
      std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT,
                                   Color::FG_RED)
                << cellInterior << Color::Modifier();
    } else {
      std::cout << cellInterior;
    }
  }

  // 输出最后一列右侧的角标，同样需要根据边界条件加上颜色
  std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED)
            << CORNER << Color::Modifier() << std::endl;
}

void CScene::init()
{
    //将所有单元格初始化为UNSELECTED状态
  memset(_map.data(), UNSELECTED, sizeof(_map));

    int col = 0;
    int row = 0;
    //把整个网格按照列的形式进行组织
    for(col = 0; col < _max_column; ++col)
    {
        CBlock column_block(_max_column);

        for(row = 0; row < _max_column; ++row)
        {
            //计算每个单元格在 _map 中的地址，并将该地址加入到 column_block 中
          column_block.push_back(_map.data() + row * _max_column + col);
        }

        _column_block[col] = column_block;
    }
    //类似的，行
    for(row = 0; row < _max_column; ++row)
    {
        CBlock row_block(_max_column);

        for(col = 0; col < _max_column; ++col)
        {
          row_block.push_back(_map.data() + row * _max_column + col);
        }

        _row_block[row] = row_block;
    }

    //按子块组织数据
    for(row = 0; row < _max_column; ++row)
    {
        for(col = 0; col < _max_column; ++col)
        {
        _xy_block[row / subBlockSize][col / subBlockSize].push_back(
            _map.data() + row * _max_column +
                                              col);
        }
    }

    return;
}

bool CScene::setCurValue(const int nCurValue, int &nLastValue)
{
    /*_cur_point.x 通常代表当前点在网格中的列号（即横向位置）。
    _cur_point.y 通常代表当前点在网格中的行号（即纵向位置）。*/
    auto& point = _map[_cur_point.x + _cur_point.y * _max_column];
    if (point.state == State::ERASED)
    {
        nLastValue = point.value;
        setValue(nCurValue);
        return true;
    }
    else
        return false;
}

void CScene::setValue(const point_t& p, const int value)
{
    _map[p.x + p.y * _max_column].value = value;
}

void CScene::setValue(const int value)
{
    auto p = _cur_point;
    this->setValue(p, value);
}

// 选择count个格子清空
void CScene::eraseRandomGrids(const int count)
{
    point_value_t p = {UNSELECTED, State::ERASED};

    std::vector<int> v(_max_column * _max_column);
    for (int i = 0; i < _max_column * _max_column; ++i) {
        v[i] = i;
    }

    for (int i = 0; i < count; ++i) {
        int r = random(0, static_cast<int>(v.size() - 1));
        _map[v[r]] = p;
        v.erase(v.begin() + r);
    }
}

bool CScene::isComplete()
{
    // 任何一个block未被填满，则肯定未完成
  for (size_t i = 0; i < _max_column * _max_column; ++i)
    {
        if (_map[i].value == UNSELECTED)
            return false;
    }

    // 验证行、列及区域的规则是否满足数独要求
    for (int row = 0; row < _max_column; ++row)
    {
      for (int col = 0; col < _max_column; ++col)
        {
            if (!_row_block[row].isValid() || 
                !_column_block[col].isValid() || 
                !_xy_block[row / subBlockSize][col / subBlockSize].isValid())
                return false;
        }
    }

    return true;
}

//将当前场景（或游戏状态）的数据保存到一个外部文件中
//1 检查文件是否已存在
//2 打开文件流并写入各种数据
//3 关闭文件并返回结果
bool CScene::save(const char *filename) {
  auto filepath = std::filesystem::path(filename);

  if (std::filesystem::exists(filepath)) {
    return false;
  }

    std::fstream fs;
    fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

    // save _map
    for (int i = 0; i < _max_column * _max_column; i++) {
        fs << _map[i].value << ' ' << static_cast<int>(_map[i].state) << std::endl;
    }

    // save _cur_point
    fs << _cur_point.x << ' ' << _cur_point.y << std::endl;

    // save _vCommand
    fs << _vCommand.size() << std::endl;
    for (CCommand command : _vCommand) {
        point_t point = command.getPoint();
        fs << point.x << ' ' << point.y << ' '
           << command.getPreValue() << ' '
           << command.getCurValue() << std::endl;
    }

    fs.close();
    return true;
}

//从文件中加载并重构出一个场景（即游戏状态或数据状态）
bool CScene::load(const char *filename) {
    //1. 检查文件是否存在
  auto filepath = std::filesystem::path(filename);
  if (!std::filesystem::exists(filepath)) {
    return false;
  }

  //2. 打开文件流
    std::fstream fs;
    fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

    // load _map
    //流对象会默认启用 std::ios::skipws 标志
    // 会跳过空白字符的，包括空格、制表符、换行符等
    for (int i = 0; i < _max_column * _max_column; i++) {
        int tmpState;
        fs >> _map[i].value >> tmpState;
        _map[i].state = static_cast<State>(tmpState);
    }

    // load _cur_point
    fs >> _cur_point.x >> _cur_point.y;

    // load _vCommand
    int commandSize;
    fs >> commandSize;
    for (int i = 0; i < commandSize; i++) {
        point_t point;
        int preValue, curValue;
        fs >> point.x >> point.y >> preValue >> curValue;
        _vCommand.emplace_back(this, point, preValue, curValue);
    }
    return true;
}


void CScene::play() {
  show();

  // 无限循环等待用户指令
  while (true) {
    // _getch() 立即捕获一个字符，不需要等待回车
    char firstChar = _getch();

    // 如果捕获的是数字键，就进入数字组合模式
    if (firstChar >= '0' && firstChar <= '9') {
      std::string inputDigits;
      inputDigits.push_back(firstChar);

      // 定义等待窗口（例如300毫秒），用于判断后续是否有连续的数字按下
      const int THRESHOLD_MS = 400;
      auto startTime = std::chrono::steady_clock::now();

      // 循环检测是否有连续按下的数字
      while (true) {
        // 如果在等待窗口内有按键
        if (_kbhit()) {
          char nextChar = _getch();
          if (nextChar >= '0' && nextChar <= '9') {
            inputDigits.push_back(nextChar);
            // 重置计时，以便允许用户稍微拖延后输入下一位数字
            startTime = std::chrono::steady_clock::now();
          } else {
            // 如果下一个按键不是数字，则暂时不做处理，
            // 或者视情况将其丢弃（因为你当前在处理数字输入）
            break;
          }
        }
        // 检查是否超出等待窗口时间
        auto now = std::chrono::steady_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                          now - startTime)
                          .count();
        if (elapsed > THRESHOLD_MS) {
          break;
        }
        // 防止CPU占用过高，暂停一小段时间
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }

      // 将组合得到的字符串转换为整数
      int number = std::atoi(inputDigits.c_str());

      // 采用新的数字执行命令
      CCommand oCommand(this);
      if (!oCommand.execute(number)) {
        std::cout << "this number can't be modified." << std::endl;
      } else {
        _vCommand.push_back(std::move(oCommand));
        show();
      }
      continue;
    }

    // 非数字的按键处理：ESC、U、方向键等保持原有处理
    if (firstChar == keyMap->ESC) {
      message(I18n::Instance().Get(I18n::Key::ASK_QUIT));
      std::string strInput;
      std::cin >> strInput;
      if (strInput[0] == 'y' || strInput[0] == 'Y') {
        message(I18n::Instance().Get(I18n::Key::ASK_SAVE));
        std::cin >> strInput;
        if (strInput[0] == 'y' || strInput[0] == 'Y') {
          do {
            message(I18n::Instance().Get(I18n::Key::ASK_SAVE_PATH));
            std::cin >> strInput;
            if (!save(strInput.c_str())) {
              message(I18n::Instance().Get(I18n::Key::FILE_EXIST_ERROR));
            } else {
              break;
            }
          } while (true);
        }
        exit(0);
      } else {
        message(I18n::Instance().Get(I18n::Key::CONTINUE));
      }
    } else if (firstChar == keyMap->U) {
      if (_vCommand.empty()) {
        message(I18n::Instance().Get(I18n::Key::UNDO_ERROR));
      } else {
        CCommand &oCommand = _vCommand.back();
        oCommand.undo();
        _vCommand.pop_back();
        show();
      }
    } else if (firstChar == keyMap->LEFT) {
      _cur_point.x = (_cur_point.x - 1) < 0 ? 0 : _cur_point.x - 1;
      show();
    } else if (firstChar == keyMap->RIGHT) {
      _cur_point.x = (_cur_point.x + 1) > (_max_column - 1) ? (_max_column - 1)
                                                            : _cur_point.x + 1;
      show();
    } else if (firstChar == keyMap->DOWN) {
      _cur_point.y = (_cur_point.y + 1) > (_max_column - 1) ? (_max_column - 1)
                                                            : _cur_point.y + 1;
      show();
    } else if (firstChar == keyMap->UP) {
      _cur_point.y = (_cur_point.y - 1) < 0 ? 0 : _cur_point.y - 1;
      show();
    } else if (firstChar == keyMap->ENTER) {
      if (isComplete()) {
        message(I18n::Instance().Get(I18n::Key::CONGRATULATION));
        getchar();
        exit(0);
      } else {
        message(I18n::Instance().Get(I18n::Key::NOT_COMPLETED));
      }
    }
  }
}

//使用暴力枚举的方式来生成能够符合数独规则的数独棋盘
void CScene::generate() {
  message(I18n::Instance().Get(I18n::Key::GENERATING_SUDOKU));
  bool solved = false;
  int attempt = 0;
  std::vector<std::vector<int>> matrix;

  // 外层循环：重复尝试直到找到一个合法的数独解答
  while (!solved) {
    attempt++;
    // 初始化空棋盘：所有位置置0
    matrix.clear();
    for (int i = 0; i < _max_column; i++)
      matrix.push_back(std::vector<int>(_max_column, 0));

    // 随机填充主对角线上（按子宫块划分）的区域
    // 对于4*4数独（subBlockSize==2），这里填充两个子块，分别是走主对角线的位置
    for (int num = 0; num < subBlockSize; num++) {
      std::vector<int> unit = shuffle_unit(_max_column);
      int start_index = num * subBlockSize;
      for (int i = start_index; i < start_index + subBlockSize; i++)
        for (int j = start_index; j < start_index + subBlockSize; j++) {
          matrix[i][j] = unit.back();
          unit.pop_back();
        }
    }

    // 利用回溯算法填充剩余空格
    // 统计所有空白位置（即值为0的位置）
    std::vector<std::tuple<int, int>> box_list;
    for (int i = 0; i < _max_column; i++)
      for (int j = 0; j < _max_column; j++)
        if (matrix[i][j] == 0) box_list.push_back(std::make_tuple(i, j));

    // 用于记录每个空格的候选数字列表
    std::map<std::string, std::vector<int>> available_num;
    int full_num = 0;  // 当前已填充空格的数量
    int empty_num = static_cast<int>(box_list.size());

    // 回溯：从第一个空格开始顺序填充
    while (full_num < empty_num) {
      // 如果回溯到第一个空格之前，说明当前配置无解
      if (full_num < 0) {
        //std::cerr << "无解，回溯到了起点之前！" << std::endl;
        break;
      }
      // 取出需要填充的位置
      std::tuple<int, int> position = box_list[full_num];
      int row = std::get<0>(position);
      int col = std::get<1>(position);
      std::string key = std::to_string(row) + "x" + std::to_string(col);
      std::vector<int> able_unit;

      // 如果该位置还未生成候选数字列表，则生成之
      if (available_num.find(key) == available_num.end()) {
        // 从1..._max_column取初始候选数字集合
        able_unit = get_unit(_max_column);
        // 排除所在子宫块内已有的数字
        int start_row = row / subBlockSize * subBlockSize;
        int start_col = col / subBlockSize * subBlockSize;
        for (int i = start_row; i < start_row + subBlockSize; i++) {
          for (int j = start_col; j < start_col + subBlockSize; j++) {
            able_unit.erase(
                std::remove(able_unit.begin(), able_unit.end(), matrix[i][j]),
                able_unit.end());
          }
        }
        // 排除行内已有的数字
        for (int i = 0; i < _max_column; i++)
          if (matrix[row][i] != 0)
            able_unit.erase(
                std::remove(able_unit.begin(), able_unit.end(), matrix[row][i]),
                able_unit.end());
        // 排除列内已有的数字
        for (int i = 0; i < _max_column; i++)
          if (matrix[i][col] != 0)
            able_unit.erase(
                std::remove(able_unit.begin(), able_unit.end(), matrix[i][col]),
                able_unit.end());
        available_num[key] = able_unit;
      } else {
        able_unit = available_num[key];
      }

      // 如果该位置没有可用的数字，则执行回溯
      if (available_num[key].empty()) {
        // 删除该位置的候选记录，并将其重置为0
        available_num.erase(key);
        matrix[row][col] = 0;
        full_num--;  // 回退到上一个已经填充的空格
        if (full_num < 0) {
          //std::cerr << "无解，已回溯至起点之前！" << std::endl;
          break;
        }
        continue;
      } else  // 若候选数字存在，则尝试填充（选择候选列表最后一个数字）
      {
        matrix[row][col] = available_num[key].back();
        available_num[key].pop_back();
        full_num++;
      }
    }  // end while (回溯填充)

    // 判断填充是否成功：当full_num>=0且full_num==empty_num时，说明所有空格都已成功填充
    if (full_num >= 0 && full_num == empty_num) {
      solved = true;
    } 
  }  // end while(!solved)

  // 填入场景
  for (int row = 0; row < _max_column; ++row) {
    for (int col = 0; col < _max_column; ++col) {
      point_t point = {row, col};
      setValue(point, matrix[row][col]);
    }
  }

  assert(isComplete());

  return;
}



//对某个网格点的数据更新过程中，只在该点状态允许时(ERASED)才能进行写入操作
bool CScene::setPointValue(const point_t &stPoint, const int nValue)
{
  auto& point = _map[stPoint.x + stPoint.y * _max_column];
    if (State::ERASED == point.state)
    {
        _cur_point = stPoint;
        setValue(nValue);
        return true;
    }
    else
        return false;
}

point_t CScene::getCurPoint()
{
    return _cur_point;
}
