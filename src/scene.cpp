#include "scene.h"

#include <memory.h>

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

// 公共常量/枚举定义（如State、KeyMode、point_t等）
#include "common.h"
// 终端显示符号定义（如CORNER、LINE、ARROW等Unicode字符）
#include "display_symbol.h"
// 国际化多语言支持
#include "i18n.h"
// 通用工具函数（随机数、清屏、无回显按键、字符串打印等）
#include "utility.inl"
// 终端颜色控制（Modifier、FG_RED、BOLD等）
#include "color.h"

/*
数独游戏核心类CScene的完整实现代码，涵盖了数独棋盘初始化、合法数独生成、终端可视化显示、用户交互
（按键操作 / 数字输入 / 撤销）、游戏进度保存 /
加载、完成校验等全量核心业务逻辑， 是整个数独游戏的 “业务中枢”——
所有游戏流程（生成棋盘→用户操作→校验完成→保存 / 退出）都通过这个类的方法 落地。
*/

// 构造函数
// index：数独维度基数（固定传3，对应9x9数独）
// _max_column：计算为index的平方，即9，代表数独的行列数
// _cur_point：初始化光标位置为(0,0)（第一行第一列）
CScene::CScene(int index)
    : _max_column(static_cast<int>(pow(index, 2)))  // index=3 时，_max_column=9
      ,
      _cur_point({0, 0})  // 初始光标在（0，0）
{
  init();  // 核心初始化：分配格子指针到行/列/3x3宫区块，建立数据关联
}

// 析构函数
// 释放按键映射对象（keyMap指向Normal/Vim子类实例），避免内存泄漏
CScene::~CScene() {
  if (keyMap) delete keyMap;  // 判空后释放，防止空指针解引用
}

// 打印完整数独棋盘（核心可视化函数）
// const修饰：保证函数不修改类成员变量
void CScene::show() const {
  cls();  // 清屏，避免终端历史内容干扰显示

  printUnderline();  // 打印棋盘顶部的分隔线（line_no=-1）

  // 获取光标位置的数字值（用于高亮显示，初始为未选择状态）
  int highlighted_num = UNSELECTED;
  // 校验光标行是否在有效范围（0-8）
  if (_cur_point.y >= 0 && _cur_point.y < _max_column) {
    // 获取光标所在行的区块对象
    const CBlock &cursor_block = _row_block[_cur_point.y];
    // 获取光标列对应的数字值
    highlighted_num = cursor_block.getNumberValue(_cur_point.x);
  }

  // 遍历每一行，逐行打印数独内容
  for (int row = 0; row < _max_column; ++row) {
    // 获取当前行的区块对象
    CBlock block = _row_block[row];
    // 光标所在行：传入光标列号，高亮显示该列；非光标行：传入-1，无高亮
    if (_cur_point.y == row)
      block.print(_cur_point.x, highlighted_num);
    else
      block.print(-1, highlighted_num);
    // 打印当前行底部的分隔线（区分3x3宫用红色粗线）
    printUnderline(row);
  }
}

// 设置按键操作模式（普通模式/Normal | Vim模式）
// mode：要切换的按键模式枚举值
void CScene::setMode(KeyMode mode) {
  switch (mode) {
    // 普通模式：WASD/方向键控制光标，对应Normal类的按键映射
    case KeyMode::NORMAL:
      keyMap = new Normal;
      break;

    // Vim模式：KHJL控制光标，对应Vim类的按键映射
    case KeyMode::VIM:
      keyMap = new Vim;
      break;
  }
}

// 打印数独棋盘的横向分隔线（美化界面，区分3x3宫）
// line_no：当前分隔线对应的行号（-1代表顶部分隔线，0-8代表对应行的底部分隔线）
// const修饰：不修改类成员变量
void CScene::printUnderline(int line_no) const {
  // 判断当前分隔线是否对应光标所在行（用于显示光标列的箭头）
  auto is_curline = (_cur_point.y == line_no);
  // 遍历9列，逐列打印分隔线符号
  for (int colunm = 0; colunm < 9; ++colunm) {
    // 打印列分隔角标：3的倍数列/顶部线/3的倍数行 → 红色粗体；否则默认样式
    if ((colunm % 3) == 0 || line_no == -1 || (line_no + 1) % 3 == 0) {
      std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT,
                                   Color::FG_RED)
                << CORNER << Color::Modifier();
    } else {
      std::cout << CORNER;
    }
    // 列中间的符号：光标列显示箭头（ARROW），否则显示横线（LINE）
    auto third_symbol = (is_curline && _cur_point.x == colunm) ? ARROW : LINE;
    // 打印列中间的横线：顶部线/3的倍数行 → 红色粗体；否则默认样式
    if (line_no == -1 || (line_no + 1) % 3 == 0) {
      std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT,
                                   Color::FG_RED)
                << LINE << third_symbol << LINE << Color::Modifier();
    } else {
      std::cout << LINE << third_symbol << LINE;
    }
  }
  // 打印最后一个角标，换行结束当前分隔线
  std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED)
            << CORNER << Color::Modifier() << std::endl;
}

// 核心初始化函数：建立行/列/3x3宫与底层_map数组的指针关联
// 实现“一份数据，三层管理”：所有区块共享_map的内存，修改_map即同步所有区块
void CScene::init() {
  // 初始化_map数组所有元素为UNSELECTED（未选择/空值）
  // memset按字节填充，要求UNSELECTED是单字节值（如0）
  memset(_map, UNSELECTED, sizeof(_map));

  int col = 0;
  int row = 0;

  // 第一步：初始化列区块（_column_block）
  // 每列对应一个CBlock，存储该列9个格子的指针
  for (col = 0; col < _max_column; ++col) {
    CBlock column_block;  // 临时列区块对象

    // 遍历列的每一行，添加当前列对应行的格子指针
    for (row = 0; row < _max_column; ++row) {
      // 计算格子在_map中的一维索引：row*9 + col
      // 取地址存入列区块，实现指针关联
      column_block.push_back(_map + row * _max_column + col);
    }

    // 将初始化好的列区块存入列数组
    _column_block[col] = column_block;
  }

  // 第二步：初始化行区块（_row_block）
  // 每行对应一个CBlock，存储该行9个格子的指针
  for (row = 0; row < _max_column; ++row) {
    CBlock row_block;  // 临时行区块对象

    // 遍历行的每一列，添加当前行对应列的格子指针
    for (col = 0; col < _max_column; ++col) {
      // 计算格子在_map中的一维索引：row*9 + col
      row_block.push_back(_map + row * _max_column + col);
    }

    // 将初始化好的行区块存入行数组
    _row_block[row] = row_block;
  }

  // 第三步：初始化3x3宫区块（_xy_block）
  // 9个3x3宫（3行3列），每个宫对应一个CBlock，存储9个格子的指针
  for (row = 0; row < _max_column; ++row) {
    for (col = 0; col < _max_column; ++col) {
      // 计算当前格子所属的3x3宫：row/3（宫行）、col/3（宫列）
      // 例如row=4 → 4/3=1（第二行宫），col=5 →5/3=1（第二列宫）
      _xy_block[row / 3][col / 3].push_back(_map + row * _max_column + col);
    }
  }

  return;
}

// 设置当前光标位置格子的数值
// nCurValue：要设置的新值
// nLastValue：输出参数，返回修改前的旧值
// 返回值：true=设置成功（格子为可编辑状态），false=设置失败（格子不可编辑）
bool CScene::setCurValue(const int nCurValue, int &nLastValue) {
  // 计算光标位置格子在_map中的一维索引：x + y*9
  auto point = _map[_cur_point.x + _cur_point.y * 9];
  // 仅当格子状态为ERASED（可编辑）时，允许修改
  if (point.state == State::ERASED) {
    nLastValue = point.value;  // 记录修改前的旧值
    setValue(nCurValue);       // 调用重载函数设置新值
    return true;               // 设置成功
  } else
    return false;  // 格子不可编辑，设置失败
}

// 重载：设置指定坐标格子的数值
// p：目标格子的坐标（x=列，y=行）
// value：要设置的数值
void CScene::setValue(const point_t &p, const int value) {
  // 计算目标格子在_map中的一维索引，直接修改数值
  _map[p.x + p.y * 9].value = value;
}

// 重载：设置当前光标位置格子的数值（无坐标参数）
// value：要设置的数值
void CScene::setValue(const int value) {
  auto p = _cur_point;       // 获取当前光标坐标
  this->setValue(p, value);  // 调用带坐标的重载函数
}

// 随机擦除指定数量的格子（实现难度控制）
// count：要擦除的格子数量（Easy=20，Normal=35，Hard=50）
// 擦除逻辑：将格子状态设为ERASED（可编辑），数值设为UNSELECTED
void CScene::eraseRandomGrids(const int count) {
  // 定义擦除后的格子状态：数值UNSELECTED，状态ERASED
  point_value_t p = {UNSELECTED, State::ERASED};

  // 初始化0-80的索引数组（对应_map的81个格子）
  std::vector<int> v(81);
  for (int i = 0; i < 81; ++i) {
    v[i] = i;
  }

  // 循环count次，随机选择格子擦除
  for (int i = 0; i < count; ++i) {
    // 生成0到当前数组长度-1的随机数（避免重复选择）
    int r = random(0, static_cast<int>(v.size() - 1));
    // 将随机选中的格子设为擦除状态
    _map[v[r]] = p;
    // 从索引数组中移除已选中的索引，避免重复擦除同一个格子
    v.erase(v.begin() + r);
  }
}

// 校验数独是否完成（填满且符合规则）
// 返回值：true=完成，false=未完成
bool CScene::isComplete() {
  // 第一步：校验所有格子是否填满（无UNSELECTED值）
  // 只要有一个格子为空，直接返回未完成
  for (size_t i = 0; i < 81; ++i) {
    if (_map[i].value == UNSELECTED) return false;
  }

  // 第二步：校验所有行/列/3x3宫是否符合数独规则（1-9不重复）
  // 遍历所有行和列，逐一校验对应区块的合法性
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      // 任意一个区块不合法，返回未完成
      if (!_row_block[row].isValid() || !_column_block[col].isValid() ||
          !_xy_block[row / 3][col / 3].isValid())
        return false;
    }
  }

  // 所有校验通过，数独完成
  return true;
}

// 保存游戏进度到文件
// filename：保存的文件路径/名称
// 返回值：true=保存成功，false=保存失败（文件已存在）
bool CScene::save(const char *filename) {
  // 将文件名转为文件系统路径对象，便于判断文件是否存在
  auto filepath = std::filesystem::path(filename);
  // 若文件已存在，返回保存失败（避免覆盖）
  if (std::filesystem::exists(filepath)) {
    return false;
  }

  // 打开文件流（注：打开模式可优化为out|trunc，当前模式兼容新建）
  std::fstream fs;
  fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

  // 第一步：保存_map数组（81个格子的数值+状态）
  for (int i = 0; i < 81; i++) {
    // 写入格式：数值 状态（枚举转int），每行一个格子
    fs << _map[i].value << ' ' << static_cast<int>(_map[i].state) << std::endl;
  }

  // 第二步：保存当前光标位置
  // 写入格式：x坐标 y坐标，单独一行
  fs << _cur_point.x << ' ' << _cur_point.y << std::endl;

  // 第三步：保存操作命令记录（用于撤销）
  // 先写入命令数量，再逐行写入每个命令的信息
  fs << _vCommand.size() << std::endl;
  for (CCommand command : _vCommand) {
    point_t point = command.getPoint();
    // 写入格式：命令对应的格子坐标x y  修改前值 修改后值
    fs << point.x << ' ' << point.y << ' ' << command.getPreValue() << ' '
       << command.getCurValue() << std::endl;
  }

  // 关闭文件流，返回保存成功
  fs.close();
  return true;
}

// 从文件加载游戏进度
// filename：要加载的文件路径/名称
// 返回值：true=加载成功，false=加载失败（文件不存在）
bool CScene::load(const char *filename) {
  // 将文件名转为文件系统路径对象，判断文件是否存在
  auto filepath = std::filesystem::path(filename);
  if (!std::filesystem::exists(filepath)) {
    return false;
  }

  // 打开文件流（注：打开模式可优化为in，当前模式兼容读取）
  std::fstream fs;
  fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

  // 第一步：加载_map数组（81个格子的数值+状态）
  for (int i = 0; i < 81; i++) {
    int tmpState;  // 临时存储状态的int值
    // 读取数值和状态int值
    fs >> _map[i].value >> tmpState;
    // 将int值转回State枚举，赋值给格子状态
    _map[i].state = static_cast<State>(tmpState);
  }

  // 第二步：加载光标位置
  fs >> _cur_point.x >> _cur_point.y;

  // 第三步：加载操作命令记录
  int commandSize;  // 命令数量
  fs >> commandSize;
  // 逐行读取命令信息，重建CCommand对象存入_vCommand
  for (int i = 0; i < commandSize; i++) {
    point_t point;           // 命令对应的格子坐标
    int preValue, curValue;  // 修改前/后值
    fs >> point.x >> point.y >> preValue >> curValue;
    // 原地构造CCommand对象（避免拷贝）
    _vCommand.emplace_back(this, point, preValue, curValue);
  }
  return true;
}

// 游戏主循环（核心交互函数）
// 处理用户按键：数字输入、光标移动、撤销、退出、保存、完成校验等
void CScene::play() {
  show();  // 初始化显示数独棋盘

  char key = '\0';  // 存储用户按下的按键
  // 无限循环，直到用户退出游戏
  while (1) {
    // 无回显读取单个按键（跨平台实现，无需回车）
    key = static_cast<char>(_getch());

    // 1. 处理数字键（0-9）：填充当前光标位置的格子
    if (key >= '0' && key <= '9') {
      // 创建命令对象（关联当前场景）
      CCommand oCommand(this);
      // 执行数字填充：key-'0'将字符转为数字（如'5'→5）
      if (!oCommand.execute(key - '0')) {
        // 填充失败（格子不可编辑），打印提示
        std::cout << "this number can't be modified." << std::endl;
      } else {
        // 填充成功，将命令对象移入操作记录（注：无移动构造时实际为拷贝）
        _vCommand.push_back(std::move(oCommand));
        show();    // 刷新显示棋盘
        continue;  // 跳过后续按键判断，直接读取下一个按键
      }
    }

    // 2. 处理ESC键：退出游戏（支持保存进度）
    if (key == keyMap->ESC) {
      // 打印多语言提示：询问是否退出
      message(I18n::Instance().Get(I18n::Key::ASK_QUIT));
      std::string strInput;  // 存储用户输入
      std::cin >> strInput;
      // 用户确认退出（y/Y）
      if (strInput[0] == 'y' || strInput[0] == 'Y') {
        // 打印多语言提示：询问是否保存
        message(I18n::Instance().Get(I18n::Key::ASK_SAVE));
        std::cin >> strInput;
        // 用户确认保存（y/Y）
        if (strInput[0] == 'y' || strInput[0] == 'Y') {
          // 循环直到输入合法的保存路径（文件不存在）
          do {
            // 打印多语言提示：询问保存路径
            message(I18n::Instance().Get(I18n::Key::ASK_SAVE_PATH));
            std::cin >> strInput;
            // 尝试保存，若失败（文件已存在）打印错误提示
            if (!save(strInput.c_str())) {
              message(I18n::Instance().Get(I18n::Key::FILE_EXIST_ERROR));
            } else {
              // 保存成功，退出循环
              break;
            }
          } while (true);
        }
        // 退出游戏进程
        exit(0);
      } else {
        // 用户取消退出，打印“继续游戏”提示
        message(I18n::Instance().Get(I18n::Key::CONTINUE));
      }
    }

    // 3. 处理U键：撤销上一步操作
    else if (key == keyMap->U) {
      // 无操作记录时，打印撤销失败提示
      if (_vCommand.empty()) {
        message(I18n::Instance().Get(I18n::Key::UNDO_ERROR));
      } else {
        // 获取最后一条操作命令
        CCommand &oCommand = _vCommand.back();
        oCommand.undo();       // 执行撤销（恢复数值）
        _vCommand.pop_back();  // 移除已撤销的命令
        show();                // 刷新显示棋盘
      }
    }

    // 4. 处理左方向键：光标左移（边界校验：不小于0）
    else if (key == keyMap->LEFT) {
      _cur_point.x = (_cur_point.x - 1) < 0 ? 0 : _cur_point.x - 1;
      show();  // 刷新显示
    }

    // 5. 处理右方向键：光标右移（边界校验：不大于8）
    else if (key == keyMap->RIGHT) {
      _cur_point.x = (_cur_point.x + 1) > 8 ? 8 : _cur_point.x + 1;
      show();  // 刷新显示
    }

    // 6. 处理下方向键：光标下移（边界校验：不大于8）
    else if (key == keyMap->DOWN) {
      _cur_point.y = (_cur_point.y + 1) > 8 ? 8 : _cur_point.y + 1;
      show();  // 刷新显示
    }

    // 7. 处理上方向键：光标上移（边界校验：不小于0）
    else if (key == keyMap->UP) {
      _cur_point.y = (_cur_point.y - 1) < 0 ? 0 : _cur_point.y - 1;
      show();  // 刷新显示
    }

    // 8. 处理回车键：校验数独是否完成
    else if (key == keyMap->ENTER) {
      // 数独已完成（填满且符合规则）
      if (isComplete()) {
        // 打印恭喜提示，等待用户按键后退出
        message(I18n::Instance().Get(I18n::Key::CONGRATULATION));
        getchar();
        exit(0);
      } else {
        // 数独未完成，打印提示
        message(I18n::Instance().Get(I18n::Key::NOT_COMPLETED));
      }
    }
  }
}

// 生成合法的9x9数独棋盘（回溯法）
// 一个场景可多次调用该函数，重新生成新的数独
void CScene::generate() {
  // Reset all grid states to INITED (Fixed) before generating new values
  for (int i = 0; i < 81; ++i) {
    _map[i].state = State::INITED;
  }

  // 临时二维数组：存储生成过程中的数独数据（0代表空格）
  std::vector<std::vector<int>> matrix;
  for (int i = 0; i < 9; i++) matrix.push_back(std::vector<int>(9, 0));

  // 初始化三个对角3x3宫（左上、中中、右下），建立基础合法结构
  // 结构示例：
  // 2 6 5 | 0 0 0 | 0 0 0
  // 3 4 1 | 0 0 0 | 0 0 0
  // 8 9 7 | 0 0 0 | 0 0 0
  // ---------------------
  // 0 0 0 | 1 9 4 | 0 0 0
  // 0 0 0 | 8 3 6 | 0 0 0
  // 0 0 0 | 5 2 7 | 0 0 0
  // ---------------------
  // 0 0 0 | 0 0 0 | 3 4 5
  // 0 0 0 | 0 0 0 | 9 6 2
  // 0 0 0 | 0 0 0 | 7 8 1

  // 第一步：初始化3个对角3x3宫格，保证基础合法性
  for (int num = 0; num < 3; num++) {
    // 打乱1-9数组，保证生成的数独随机
    std::vector<int> unit = shuffle_unit();
    // 计算对角宫的起始索引（0/3/6）
    int start_index = num * 3;
    // 填充当前对角宫的9个格子
    for (int i = start_index; i < start_index + 3; i++)
      for (int j = start_index; j < start_index + 3; j++) {
        // 取数组最后一个元素填充（保证顺序随机）
        matrix[i][j] = unit.back();
        unit.pop_back();  // 移除已使用的数字
      }
  }

  // 第二步：收集所有空格（值为0）的坐标，存入列表
  std::vector<std::tuple<int, int>> box_list;
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++)
      if (matrix[i][j] == 0) box_list.push_back(std::make_tuple(i, j));

  // 第三步：回溯法填充所有空格
  // available_num：记录每个空格的可用数字（行/列/宫不重复的数字）
  std::map<std::string, std::vector<int>> available_num{};
  int full_num = 0;  // 已填充的空格数（从0开始）
  int empty_num = static_cast<int>(box_list.size());  // 总空格数
  // 循环直到所有空格填充完成
  while (full_num < empty_num) {
    // 获取当前要填充的空格坐标（从列表中按顺序取）
    std::tuple<int, int> position = box_list[full_num];
    int row = std::get<0>(position);  // 行坐标
    int col = std::get<1>(position);  // 列坐标
    std::vector<int> able_unit;       // 存储当前空格的可用数字
    // 生成空格的唯一标识（行x列），用于map的key
    std::string key = std::to_string(row) + "x" + std::to_string(col);

    // 若当前空格未计算过可用数字，重新计算
    if (available_num.find(key) == available_num.end()) {
      // 初始化可用数字为1-9
      able_unit = get_unit();

      // 1. 移除当前3x3宫内已存在的数字
      for (int i = row / 3 * 3; i < row / 3 * 3 + 3; i++) {
        for (int j = col / 3 * 3; j < col / 3 * 3 + 3; j++) {
          // 移除已存在的数字（erase+remove配合删除指定值）
          able_unit.erase(
              std::remove(able_unit.begin(), able_unit.end(), matrix[i][j]),
              able_unit.end());
        }
      }

      // 2. 移除当前行内已存在的数字
      for (int i = 0; i < 9; i++)
        if (matrix[row][i] != 0)
          able_unit.erase(
              std::remove(able_unit.begin(), able_unit.end(), matrix[row][i]),
              able_unit.end());

      // 3. 移除当前列内已存在的数字
      for (int i = 0; i < 9; i++)
        if (matrix[i][col] != 0)
          able_unit.erase(
              std::remove(able_unit.begin(), able_unit.end(), matrix[i][col]),
              able_unit.end());

      // 将计算好的可用数字存入map，避免重复计算
      available_num[key] = able_unit;
    } else {
      // 已计算过可用数字，直接从map中获取
      able_unit = available_num[key];
    }

    // 回溯逻辑：当前空格无可用数字，回退到上一个空格
    if (available_num[key].size() <= 0) {
      full_num -= 1;  // 回退到上一个空格
      // 清空当前空格的可用数字记录（重新计算）
      if (available_num.find(key) != available_num.end())
        available_num.erase(key);
      matrix[row][col] = 0;  // 清空当前空格的数值
      continue;              // 跳过后续逻辑，重新处理上一个空格
    } else                   // 有可用数字，填充最后一个数字（保证随机性）
    {
      matrix[row][col] = available_num[key].back();  // 取最后一个可用数字
      available_num[key].pop_back();                 // 移除已使用的数字
      full_num += 1;                                 // 填充下一个空格
    }
  }

  // 第四步：将生成好的数独数据填入场景的_map数组
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      point_t point = {row, col};         // 构造坐标对象
      setValue(point, matrix[row][col]);  // 设置数值
    }
  }

  // 断言：生成的数独必须是完整合法的（Debug模式下生效）
  assert(isComplete());

  return;
}

// 设置指定坐标格子的数值（带状态校验）
// stPoint：目标格子坐标 (1-based from GUI: x=1..9, y=1..9)
// nValue：要设置的数值
// 返回值：true=设置成功（格子可编辑），false=设置失败
bool CScene::setPointValue(const point_t &stPoint, const int nValue) {
  // Convert 1-based GUI coordinates to 0-based internal coordinates for index
  // calculation
  int x = stPoint.x - 1;
  int y = stPoint.y - 1;

  if (x < 0 || x > 8 || y < 0 || y > 8) return false;

  // Calculte 0-based index: row * 9 + col => y * 9 + x
  int index = y * 9 + x;
  auto &point = _map[index];

  // 仅可编辑状态（ERASED）的格子允许修改
  if (State::ERASED == point.state) {
    _cur_point = {x, y};  // Update internal cursor to 0-based

    // Direct value update (bypass setValue overload to avoid
    // confusion/recursion if any)
    point.value = nValue;
    return true;
  } else {
    return false;  // Fixed cell or invalid
  }
}

// 获取当前光标位置的坐标
// 返回值：point_t结构体（x=列，y=行）
point_t CScene::getCurPoint() { return _cur_point; }

int CScene::getValue(int x, int y) const {
  if (x < 1 || x > 9 || y < 1 || y > 9) return 0;
  return _map[(y - 1) * 9 + (x - 1)].value;
}

State CScene::getState(int x, int y) const {
  if (x < 1 || x > 9 || y < 1 || y > 9) return State::ERASED;  // Default safe
  return _map[(y - 1) * 9 + (x - 1)].state;
}

bool CScene::undo() {
  if (_vCommand.empty()) {
    return false;
  }
  CCommand &oCommand = _vCommand.back();
  oCommand.undo();       // 执行撤销（恢复数值）
  _vCommand.pop_back();  // 移除已撤销的命令
  return true;
}
