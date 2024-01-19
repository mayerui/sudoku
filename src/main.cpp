#include <cstring>
#include <iostream>

#include "input.h"
#include "scene.h"
#include "test.h"
#include "utility.inl"


#define _TEST_ 0

static void printHelp() {
    std::cout << std::endl;
    std::cout << "sudoku - a little game in command line" << std::endl
              << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "\t sudoku [-l <progressFile>]" << std::endl << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t -l <path> \t specify path of progress file to load, optional." << std::endl
              << std::endl;
}

int main(int argc, char **argv)
{
#if _TEST_
    test_case1();
    getchar();
#else
    CScene scene;

    if (argc == 1) {
        int eraseGridNumber = inputDifficulty();
        scene.generate();
        scene.eraseRandomGrids(eraseGridNumber);
    }
    else if (argc == 3 && !strcmp(argv[1], "-l")) {
      // load saved game progress
      if (!scene.load(argv[2])) {
        message("load progress failed!");
        return 0;
      }
    }
    else {
        printHelp();
        return 0;
    }

    scene.setMode(inputKeyMode());

    scene.play();
#endif

    return 0;
}