#ifndef _SUDOKU_SOLVER_H_
#define _SUDOKU_SOLVER_H_

#include <vector>
class SudokuSolver {
 private:
  std::vector<std::vector<int>> board;
  std::vector<std::vector<int>> solution;

 public:
  SudokuSolver(/* args */);
  std::vector<std::vector<int>> getSolution() const;

  int getHint(int x, int y) const;

  void setBoard(std::vector<std::vector<int>> newBoard) {
    this->board = newBoard;
    this->solution = getSolution();
  };
};

#endif