#ifndef _SUDOKU_INPUT_H_
#define _SUDOKU_INPUT_H_

#include "common.h"

/*
初始化阶段的输入交互函数
*/

//获取用户输入的难度选择
int inputDifficulty();
//获取用户输入的按键模式选择
KeyMode inputKeyMode();
//获取用户输入的语言选择
void InputLanguage();

#endif