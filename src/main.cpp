
#include <QApplication>

#include "gui/mainwindow.h"

int main(int argc, char *argv[]) {
  // Fix console output on Windows
  // #ifdef WIN32
  //   if (AttachConsole(ATTACH_PARENT_PROCESS)) {
  //     freopen("CONOUT$", "w", stdout);
  //     freopen("CONOUT$", "w", stderr);
  //   }
  // #endif

  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
