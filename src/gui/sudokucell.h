#ifndef SUDOKUCELL_H
#define SUDOKUCELL_H

#include <QLabel>
#include <QMouseEvent>

class SudokuCell : public QLabel {
  Q_OBJECT

 public:
  explicit SudokuCell(int x, int y, QWidget *parent = nullptr);

  void setValue(int value);
  void setIsFixed(bool fixed);
  void setSelected(bool selected);
  void setConflict(bool conflict);
  void setBlockColor(const QString &color);  // Added method

  int getX() const { return m_x; }
  int getY() const { return m_y; }

 signals:
  void cellClicked(int x, int y);

 protected:
  void mousePressEvent(QMouseEvent *event) override;

 private:
  void updateStyle();

  int m_x;
  int m_y;
  int m_value;
  bool m_isFixed;
  bool m_isSelected;
  bool m_isConflict;     // Renamed from m_hasConflict
  QString m_blockColor;  // Added member
};

#endif  // SUDOKUCELL_H
