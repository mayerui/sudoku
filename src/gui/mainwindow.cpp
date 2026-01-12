#include "mainwindow.h"

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  m_currentPoint = {1, 1};  // Start at 1,1
  m_scene.generate();       // Generate initial game
  setupUi();
  updateGrid();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi() {
  QWidget *centralWidget = new QWidget(this);
  QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

  // 1. Sudoku Grid
  QGridLayout *gridLayout = new QGridLayout();
  gridLayout->setSpacing(0);

  for (int y = 0; y < 9; ++y) {
    for (int x = 0; x < 9; ++x) {
      // Scene uses 1-based indexing for logic, but we use 0-based for array
      m_cells[y][x] = new SudokuCell(x + 1, y + 1);
      connect(m_cells[y][x], &SudokuCell::cellClicked, this,
              &MainWindow::onCellClicked);
      gridLayout->addWidget(m_cells[y][x], y, x);
    }
  }

  // Add spacing to separate 3x3 blocks
  gridLayout->setHorizontalSpacing(2);
  gridLayout->setVerticalSpacing(2);
  // Note: Creating actual large gaps in QGridLayout is tricky without nested
  // layouts, we use a simple uniform spacing for now.

  mainLayout->addLayout(gridLayout);

  // Control Panel
  QVBoxLayout *controlLayout = new QVBoxLayout();

  // Difficulty Selector
  m_difficultyCombo = new QComboBox(this);
  m_difficultyCombo->addItem("Easy");
  m_difficultyCombo->addItem("Normal");
  m_difficultyCombo->addItem("Hard");
  m_difficultyCombo->setCurrentIndex(1);  // Default to Normal
  controlLayout->addWidget(new QLabel("Difficulty:", this));
  controlLayout->addWidget(m_difficultyCombo);

  QPushButton *btnNewGame = new QPushButton("New Game", this);
  connect(btnNewGame, &QPushButton::clicked, this, &MainWindow::onNewGame);
  controlLayout->addWidget(btnNewGame);

  m_undoButton = new QPushButton("Undo", this);
  connect(m_undoButton, &QPushButton::clicked, this, &MainWindow::onUndo);
  controlLayout->addWidget(m_undoButton);

  m_checkButton = new QPushButton("Check / Finish", this);
  connect(m_checkButton, &QPushButton::clicked, this, &MainWindow::onCheckGame);
  controlLayout->addWidget(m_checkButton);

  controlLayout->addStretch();

  QLabel *instructions = new QLabel(
      "Controls:\nClick cell to select\nType 1-9 to fill\n0/Del to clear");
  controlLayout->addWidget(instructions);

  mainLayout->addLayout(controlLayout);

  centralWidget->setLayout(mainLayout);
  setCentralWidget(centralWidget);
  setWindowTitle("Sudoku Qt6");
}

void MainWindow::updateGrid() {
  for (int y = 1; y <= 9; ++y) {
    for (int x = 1; x <= 9; ++x) {
      int value = m_scene.getValue(x, y);
      State state = m_scene.getState(x, y);
      m_cells[y - 1][x - 1]->setValue(value);

      bool isFixed = (state == State::INITED);
      m_cells[y - 1][x - 1]->setIsFixed(isFixed);

      updateCellColor(x, y);
    }
  }
}

void MainWindow::updateCellColor(int x, int y) {
  SudokuCell *cell = m_cells[y - 1][x - 1];

  // Determine background color based on 3x3 block
  int blockX = (x - 1) / 3;
  int blockY = (y - 1) / 3;
  bool isDarker = (blockX + blockY) % 2 != 0;

  QString bgColor = isDarker ? "#e0e0e0" : "#ffffff";

  point_t cur = m_currentPoint;
  bool isSelected = (cur.x == x && cur.y == y);

  cell->setBlockColor(bgColor);  // New method we will add to SudokuCell
  cell->setSelected(isSelected);
}

void MainWindow::onCellClicked(int x, int y) {
  m_currentPoint = {x, y};
  updateGrid();
}

void MainWindow::onNewGame() {
  m_scene.generate();

  int difficulty = m_difficultyCombo->currentIndex();
  int eraseCount = 20;  // Easy
  if (difficulty == 1)
    eraseCount = 35;  // Normal
  else if (difficulty == 2)
    eraseCount = 50;  // Hard

  m_scene.eraseRandomGrids(eraseCount);
  m_currentPoint = {1, 1};
  updateGrid();
  m_checkButton->setText("Check / Finish");
}

void MainWindow::onUndo() {
  if (m_scene.undo()) {
    updateGrid();
    m_checkButton->setText("Check / Finish");
  }
}

void MainWindow::onCheckGame() {
  if (m_scene.isComplete()) {
    QMessageBox::information(this, "Sudoku",
                             "Congratulations! You solved the puzzle!");
    m_checkButton->setText("Solved!");
  } else {
    QMessageBox::warning(this, "Sudoku", "Not correct or incomplete yet.");
    m_checkButton->setText("Not yet...");
  }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  int key = event->key();
  int num = -1;

  if (key >= Qt::Key_1 && key <= Qt::Key_9) {
    num = key - Qt::Key_0;
  } else if (key == Qt::Key_0 || key == Qt::Key_Backspace ||
             key == Qt::Key_Delete) {
    num = 0;
  } else if (key == Qt::Key_Up) {
    if (m_currentPoint.y > 1) m_currentPoint.y--;
    updateGrid();
    return;
  } else if (key == Qt::Key_Down) {
    if (m_currentPoint.y < 9) m_currentPoint.y++;
    updateGrid();
    return;
  } else if (key == Qt::Key_Left) {
    if (m_currentPoint.x > 1) m_currentPoint.x--;
    updateGrid();
    return;
  } else if (key == Qt::Key_Right) {
    if (m_currentPoint.x < 9) m_currentPoint.x++;
    updateGrid();
    return;
  }

  if (num != -1) {
    // We need to bypass the CScene::play loop and call logic directly.
    // CScene doesn't expose a simple "setCellValue" that respects rules
    // directly easily without `setCurValue`. We need to update CScene's
    // internal current point first potentially, OR we just use setPointValue if
    // we modify CScene to allow it. Looking at CScene: bool setPointValue(const
    // point_t&, const int); is public!

    m_scene.setPointValue(m_currentPoint, num);

    // Also check if game is complete?
    // if (m_scene->isComplete()) { ... }

    updateGrid();
  } else {
    QMainWindow::keyPressEvent(event);
  }
}
