#include <cstring>
#include <iostream>

#include "i18n.h"
#include "input.h"
#include "scene.h"
#include "minesweeper.h"
#include "system_env.hpp"
#include "utility.inl"

static void printHelp() {
    std::cout << std::endl;
    std::cout << "Game Menu - Choose your game:" << std::endl << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "\t game [-l <progressFile>] [-m <mode>]" << std::endl << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t -l <path> \t specify path of progress file to load, optional." << std::endl;
    std::cout << "\t -m <mode> \t specify game mode: 1 for Sudoku, 2 for Minesweeper" << std::endl << std::endl;
}

// 选择游戏模式
int selectGameMode() {
    std::string mode;
    do {
        std::cout << "选择游戏模式 / Select game mode:" << std::endl;
        std::cout << "1. 数独 / Sudoku" << std::endl;
        std::cout << "2. 扫雷 / Minesweeper" << std::endl;
        std::cout << "请选择 / Please choose (1-2): ";
        
        std::cin >> mode;
        if(mode == "1" || mode == "2") {
            return std::stoi(mode);
        }
        message(I18n::Instance().Get(I18n::Key::INPUT_ERROR));
    } while(true);
}

int main(int argc, char **argv)
{
    SetSystemEnv();
    InputLanguage();

    int gameMode = 1;  // 默认为数独模式
    const char* loadFile = nullptr;

    // 处理命令行参数
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            loadFile = argv[++i];
        }
        else if(strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            gameMode = std::stoi(argv[++i]);
        }
        else {
            printHelp();
            return 0;
        }
    }

    // 如果没有通过命令行指定游戏模式，则提示用户选择
    if(argc == 1 || (argc == 3 && loadFile != nullptr)) {
        gameMode = selectGameMode();
    }

    KeyMode keyMode = inputKeyMode();

    if(gameMode == 1) {  // 数独模式
        CScene scene;
        scene.setMode(keyMode);

        if(loadFile) {
            if(!scene.load(loadFile)) {
                message(I18n::Instance().Get(I18n::Key::LOAD_PROGRESS_FAIL));
                return 0;
            }
        } else {
            int eraseGridNumber = inputDifficulty();
            scene.generate();
            scene.eraseRandomGrids(eraseGridNumber);
        }

        scene.play();
    }
    else if(gameMode == 2) {  // 扫雷模式
        CMinesweeper minesweeper(9, 9, 10);  // 默认9x9，10个地雷
        minesweeper.setMode(keyMode);
        
        if(loadFile) {
            if(!minesweeper.load(loadFile)) {
                message(I18n::Instance().Get(I18n::Key::LOAD_PROGRESS_FAIL));
                return 0;
            }
        }
        
        minesweeper.play();
    }

    return 0;
}