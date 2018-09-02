#ifndef _SUDOKU_UTILITY_INL_
#define _SUDOKU_UTILITY_INL_

#include <cstdlib>
#include <ctime>
#include <cassert>

//not real random,return number between [begin,end]
inline unsigned int random(int begin, int end)
{
    assert(end > begin && begin >= 0);
    srand(time(NULL));
    return (unsigned int)rand() % (end - begin + 1) + begin;
}

//网上找的均匀化随机数算法,不含max
inline int AverageRandom(int min, int max)
{
    int minInteger = min * 10000;
    int maxInteger = max * 10000;
    int randInteger = rand() * rand();
    int diffInteger = maxInteger - minInteger;
    int resultInteger = randInteger % diffInteger + minInteger;

    return (resultInteger / 10000);
}

#endif