#include "test.h"
#include <time.h>
#include <iostream>

using namespace std;

//生成棋盘,10s
void test_case1()
{
    CScene scene;

    time_t begin, end;
    time(&begin);
    scene.generate();
    time(&end);

    scene.show();
    cout << end - begin << endl;
}