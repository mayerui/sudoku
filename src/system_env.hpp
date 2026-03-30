#pragma once

#include <iostream>

#if _WIN32
#include <Windows.h>
#endif

inline void SetWindowsEnv() {
#if _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif
}

inline void SetSystemEnv() {
  SetWindowsEnv();
  // 禁用输出缓冲，加快输出速度，减少卡顿
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}
