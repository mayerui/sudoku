#pragma once

#if _WIN32
#include <Windows.h>
#endif

inline void SetWindowsEnv() {
#if _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif
}
inline void SetConsoleWindowSize(int width, int height) {
#if _WIN32
  HWND hwnd = GetConsoleWindow();  // 获取控制台窗口句柄
  if (hwnd != NULL) {
    // 设置窗口的新尺寸
    MoveWindow(hwnd, 100, 100, width, height, TRUE);
  }
#endif
}
inline void SetSystemEnv() { 
    SetWindowsEnv(); 
    SetConsoleWindowSize(800,400);
}
