#include "SudokuSolver.h"
using namespace std;

SudokuSolver::SudokuSolver() {}

bool isValid(const std::vector<std::vector<int>> &board, int row, int col, int num) {
  // 检查行
  for (int i = 0; i < 9; ++i) {
      if (board[row][i] == num) {
          return false;
      }
  }
  // 检查列
  for (int i = 0; i < 9; ++i) {
      if (board[i][col] == num) {
          return false;
      }
  }
  // 检查3x3小宫格
  int startRow = (row / 3) * 3;
  int startCol = (col / 3) * 3;
  for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
          if (board[startRow + i][startCol + j] == num) {
              return false;
          }
      }
  }
  return true;
}

// 递归回溯求解数独
bool solveSudoku(vector<vector<int>>& board) {
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      if (board[row][col] == 0) {             // 如果是空格
        for (int num = 1; num <= 9; ++num) {  // 尝试填入1到9
          if (isValid(board, row, col, num)) {
            board[row][col] = num;  // 填入数字
            if (solveSudoku(board)) {
              return true;  // 如果成功，返回true
            }
            board[row][col] = 0;  // 回溯
          }
        }
        return false;  // 如果没有数字可以填入，返回false
      }
    }
  }
  return true;  // 如果没有空格，说明已经解决
}

int SudokuSolver::getHint(int x, int y) const {
  if (this->solution.size() > 0) {
    return this->solution[x][y];
  } else {
    return -1;
  }
}

std::vector<std::vector<int>> SudokuSolver::getSolution() const {
  auto res = vector<vector<int>>(this->board);
  return solveSudoku(res) ? res : vector<vector<int>>();
}
