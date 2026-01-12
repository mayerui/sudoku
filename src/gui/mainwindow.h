#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>

#include "../scene.h"
#include "sudokucell.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 protected:
  void keyPressEvent(QKeyEvent* event) override;

 private slots:
  void onCellClicked(int x, int y);
  void onNewGame();
  void onCheckGame();
  void onUndo();

 private:
  void updateGrid();
  void setupUi();
  void updateCellColor(int x, int y);

  CScene m_scene;
  SudokuCell* m_cells[9][9];
  QComboBox* m_difficultyCombo;
  QPushButton* m_undoButton;
  QPushButton* m_checkButton;
  point_t m_currentPoint;
};

#endif  // MAINWINDOW_H
