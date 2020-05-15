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
    getchar();
#else
    int eraseGridNumber = inputDifficulty();

    CScene scene;
    
    scene.generate();
    
    scene.eraseRandomGrids(eraseGridNumber);

    scene.play();
#endif

    return 0;
}