#include "sudokucell.h"

#include <QFont>

SudokuCell::SudokuCell(int x, int y, QWidget *parent)
    : QLabel(parent),
      m_x(x),
      m_y(y),
      m_value(0),
      m_isFixed(false),
      m_isSelected(false),
      m_isConflict(false),
      m_blockColor("#ffffff") {
  setAlignment(Qt::AlignCenter);
  setFixedSize(40, 40);  // Cell size
  QFont f = font();
  f.setPointSize(16);
  setFont(f);
  updateStyle();
}

void SudokuCell::setValue(int value) {
  m_value = value;
  if (value == 0) {
    setText("");
  } else {
    setText(QString::number(value));
  }
  updateStyle();
}

void SudokuCell::setIsFixed(bool fixed) {
  m_isFixed = fixed;
  updateStyle();
}

void SudokuCell::setSelected(bool selected) {
  m_isSelected = selected;
  updateStyle();
}

void SudokuCell::setConflict(bool conflict) {
  m_isConflict = conflict;  // Changed to m_isConflict
  updateStyle();
}

void SudokuCell::setBlockColor(const QString &color) {
  m_blockColor = color;
  updateStyle();
}

void SudokuCell::mousePressEvent(QMouseEvent *event) {
  // Removed the if (event->button() == Qt::LeftButton) check
  emit cellClicked(m_x, m_y);
  QLabel::mousePressEvent(event);
}

void SudokuCell::updateStyle() {
  QString style =
      QString("border: 1px solid black; font-size: 20px; background-color: %1;")
          .arg(m_blockColor);

  // Text color
  if (m_isFixed) {
    style += "color: black; font-weight: bold;";
  } else {
    style += "color: blue;";  // Changed color for user input
  }

  // Overwrite background for selection/conflict
  if (m_isSelected) {
    style += "background-color: #a0d0ff;";  // Light blue selection
  } else if (m_isConflict) {                // Changed to m_isConflict
    style += "background-color: #ffcccc;";  // Light red conflict
  }

  setStyleSheet(style);
}
