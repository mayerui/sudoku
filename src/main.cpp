#include <iostream>
#include <string>
#include "scene.h"
#include "input.h"
#include "test.h"

#define _TEST_ 0

int main()
{
#if _TEST_
    test_case1();
    system("pause");
#else
    CScene scene;

    int erase_grid_number = inputDifficulty();
    std::cout << &erase_grid_number << std::endl;
    //scene.print();

    scene.generate();
    scene.eraseRandomGrids(erase_grid_number);

    while (1)
    {
        scene.play();
    }
#endif

    return 0;
}