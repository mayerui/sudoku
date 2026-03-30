#pragma once

#if _WIN32
#include <Windows.h>
#endif

inline void SetWindowsEnv() {
#if _WIN32					//检测是否在 Windows 平台上编译
  SetConsoleOutputCP(CP_UTF8);	//将 Windows 控制台的输出代码页设置为 UTF-8
#endif
}

inline void SetSystemEnv() { SetWindowsEnv(); }
