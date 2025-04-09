#include "minesweeper.h"
#include "display_symbol.h"
#include "i18n.h"
#include "utility.inl"
#include <queue>
#include <iostream>
#include <fstream>
#include <filesystem>

CMinesweeper::CMinesweeper(int width, int height, int mines)
    : _width(width)
    , _height(height)
    , _total_mines(mines)
    , _remaining_cells(width * height - mines)
    , _cur_point({0, 0})
    , _first_move(true)
    , _game_over(false)
    , keyMap(nullptr)
{
    init();
}

CMinesweeper::~CMinesweeper()
{
    if(keyMap) delete keyMap;
}

void CMinesweeper::setMode(KeyMode mode)
{
    if(keyMap) delete keyMap;
    
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

void CMinesweeper::init() {
    // 初始化游戏板
    _board.resize(_height, std::vector<MineCell>(_width));
    for(int y = 0; y < _height; ++y) {
        for(int x = 0; x < _width; ++x) {
            _board[y][x] = {false, 0, MineState::HIDDEN};
        }
    }
}

void CMinesweeper::generate() {
    // 生成地雷
    int mines_placed = 0;
    while(mines_placed < _total_mines) {
        int x = random(0, _width - 1);
        int y = random(0, _height - 1);
        
        // 确保第一次点击的位置及其周围没有地雷
        if(_first_move && abs(x - _cur_point.x) <= 1 && abs(y - _cur_point.y) <= 1) {
            continue;
        }
        
        if(!_board[y][x].is_mine) {
            _board[y][x].is_mine = true;
            mines_placed++;
        }
    }

    // 计算每个格子周围的地雷数
    for(int y = 0; y < _height; ++y) {
        for(int x = 0; x < _width; ++x) {
            if(!_board[y][x].is_mine) {
                _board[y][x].adjacent_mines = countAdjacentMines(x, y);
            }
        }
    }
}

int CMinesweeper::countAdjacentMines(int x, int y) const {
    int count = 0;
    for(int dy = -1; dy <= 1; ++dy) {
        for(int dx = -1; dx <= 1; ++dx) {
            int nx = x + dx;
            int ny = y + dy;
            if(isValid(nx, ny) && _board[ny][nx].is_mine) {
                count++;
            }
        }
    }
    return count;
}

bool CMinesweeper::isValid(int x, int y) const {
    return x >= 0 && x < _width && y >= 0 && y < _height;
}

void CMinesweeper::show() const {
    cls();
    
    // 显示游戏信息
    std::cout << "Mines: " << _total_mines << " | Remaining safe cells: " << _remaining_cells << std::endl;
    
    // 显示游戏板
    // 显示第一行上边框
    std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED);
    for(int x = 0; x < _width; ++x) {
        std::cout << CORNER << LINE << LINE << LINE;
    }
    std::cout << CORNER << Color::Modifier() << std::endl;
    
    // 显示每一行内容
    for(int y = 0; y < _height; ++y) {
        // 显示格子内容
        for(int x = 0; x < _width; ++x) {
            const MineCell& cell = _board[y][x];
            std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) 
                     << PIPE << Color::Modifier() << " ";
            
            // 显示不同状态的格子
            if(_cur_point.x == x && _cur_point.y == y) {
                std::cout << Color::Modifier(Color::BOLD, Color::BG_CYAN, Color::FG_BLACK);
            }
            
            switch(cell.state) {
                case MineState::HIDDEN:
                    std::cout << "■";
                    break;
                case MineState::FLAGGED:
                    std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) << "F";
                    break;
                case MineState::REVEALED:
                    if(cell.is_mine) {
                        std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) << "*";
                    } else if(cell.adjacent_mines == 0) {
                        std::cout << " ";
                    } else {
                        std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_GREEN) 
                                << cell.adjacent_mines;
                    }
                    break;
                case MineState::EXPLODED:
                    std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) << "X";
                    break;
            }
            std::cout << Color::Modifier() << " ";
        }
        std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) 
                 << PIPE << Color::Modifier() << std::endl;
        
        // 显示分隔线
        std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED);
        for(int x = 0; x < _width; ++x) {
            std::cout << CORNER << LINE;
            // 在光标位置的下方显示^
            if(_cur_point.x == x && _cur_point.y == y) {
                std::cout << ARROW;
            } else {
                std::cout << LINE;
            }
            std::cout << LINE;
        }
        std::cout << CORNER << Color::Modifier() << std::endl;
    }
}

void CMinesweeper::revealCell(int x, int y) {
    if(!isValid(x, y) || _board[y][x].state != MineState::HIDDEN) {
        return;
    }

    _board[y][x].state = MineState::REVEALED;
    if(!_board[y][x].is_mine) {
        _remaining_cells--;
    }

    // 如果是空格子，递归揭示周围的格子
    if(_board[y][x].adjacent_mines == 0) {
        for(int dy = -1; dy <= 1; ++dy) {
            for(int dx = -1; dx <= 1; ++dx) {
                revealCell(x + dx, y + dy);
            }
        }
    }
}

bool CMinesweeper::reveal(int x, int y) {
    if(!isValid(x, y) || _board[y][x].state == MineState::FLAGGED) {
        return false;
    }

    if(_first_move) {
        _first_move = false;
        generate();
    }

    if(_board[y][x].is_mine) {
        _board[y][x].state = MineState::EXPLODED;
        _game_over = true;
        revealAllMines();
        return false;
    }

    revealCell(x, y);
    return true;
}

void CMinesweeper::toggleFlag(int x, int y) {
    if(!isValid(x, y) || _board[y][x].state == MineState::REVEALED) {
        return;
    }

    if(_board[y][x].state == MineState::HIDDEN) {
        _board[y][x].state = MineState::FLAGGED;
    } else if(_board[y][x].state == MineState::FLAGGED) {
        _board[y][x].state = MineState::HIDDEN;
    }
}

void CMinesweeper::revealAllMines() {
    for(int y = 0; y < _height; ++y) {
        for(int x = 0; x < _width; ++x) {
            if(_board[y][x].is_mine && _board[y][x].state != MineState::EXPLODED) {
                _board[y][x].state = MineState::REVEALED;
            }
        }
    }
}

bool CMinesweeper::isGameOver() const {
    return _game_over;
}

bool CMinesweeper::isWin() const {
    return _remaining_cells == 0;
}

bool CMinesweeper::save(const char* filename) {
    auto filepath = std::filesystem::path(filename);
    if (std::filesystem::exists(filepath)) {
        return false;
    }

    std::fstream fs;
    fs.open(filename, std::fstream::out);
    if (!fs.is_open()) {
        return false;
    }

    // 保存基本信息
    fs << _width << ' ' << _height << ' ' << _total_mines << ' ' 
       << _remaining_cells << ' ' << _first_move << ' ' << _game_over << std::endl;
    
    // 保存当前光标位置
    fs << _cur_point.x << ' ' << _cur_point.y << std::endl;
    
    // 保存游戏板状态
    for(int y = 0; y < _height; ++y) {
        for(int x = 0; x < _width; ++x) {
            const MineCell& cell = _board[y][x];
            fs << cell.is_mine << ' ' 
               << cell.adjacent_mines << ' ' 
               << static_cast<int>(cell.state) << std::endl;
        }
    }

    fs.close();
    return true;
}

bool CMinesweeper::load(const char* filename) {
    auto filepath = std::filesystem::path(filename);
    if (!std::filesystem::exists(filepath)) {
        return false;
    }

    std::fstream fs;
    fs.open(filename, std::fstream::in);
    if (!fs.is_open()) {
        return false;
    }

    // 读取基本信息
    fs >> _width >> _height >> _total_mines 
       >> _remaining_cells >> _first_move >> _game_over;
    
    // 读取当前光标位置
    fs >> _cur_point.x >> _cur_point.y;
    
    // 初始化游戏板
    _board.resize(_height, std::vector<MineCell>(_width));
    
    // 读取游戏板状态
    for(int y = 0; y < _height; ++y) {
        for(int x = 0; x < _width; ++x) {
            bool is_mine;
            int adjacent_mines;
            int state;
            fs >> is_mine >> adjacent_mines >> state;
            
            _board[y][x].is_mine = is_mine;
            _board[y][x].adjacent_mines = adjacent_mines;
            _board[y][x].state = static_cast<MineState>(state);
        }
    }

    fs.close();
    return true;
}

void CMinesweeper::play() {
    show();

    while(!isGameOver() && !isWin()) {
        char key = _getch();
        
        if(key == keyMap->ESC) {
            message(I18n::Instance().Get(I18n::Key::ASK_QUIT));
            std::string strInput;
            std::cin >> strInput;
            if(strInput[0] == 'y' || strInput[0] == 'Y') {
                message(I18n::Instance().Get(I18n::Key::ASK_SAVE));
                std::cin >> strInput;
                if(strInput[0] == 'y' || strInput[0] == 'Y') {
                    do {
                        message(I18n::Instance().Get(I18n::Key::ASK_SAVE_PATH));
                        std::cin >> strInput;
                        if(!save(strInput.c_str())) {
                            message(I18n::Instance().Get(I18n::Key::FILE_EXIST_ERROR));
                        } else {
                            break;
                        }
                    } while(true);
                }
                return;
            }
        }
        else if(key == ' ' || key == '\r') {  // 空格或回车键揭示格子
            reveal(_cur_point.x, _cur_point.y);
            show();
        }
        else if(key == 'f' || key == 'F') {  // F键标记地雷
            toggleFlag(_cur_point.x, _cur_point.y);
            show();
        }
        else if(key == keyMap->LEFT) {
            _cur_point.x = (_cur_point.x - 1 + _width) % _width;
            show();
        }
        else if(key == keyMap->RIGHT) {
            _cur_point.x = (_cur_point.x + 1) % _width;
            show();
        }
        else if(key == keyMap->UP) {
            _cur_point.y = (_cur_point.y - 1 + _height) % _height;
            show();
        }
        else if(key == keyMap->DOWN) {
            _cur_point.y = (_cur_point.y + 1) % _height;
            show();
        }
    }

    if(isWin()) {
        message("恭喜你赢了！");
    } else {
        message("游戏结束！");
    }
    _getch();
} 
