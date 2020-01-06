# sudoku
C++实现的跨平台数独游戏，命令行操作易上手，可以在开发间隙用来放松身心。数百行代码，初学者也可以轻松掌握。

特性:
1. 跨平台 : Linux/Windows
---
构建：
1. Windows下一键编译:build.bat
2. Linux下一键构建:sh build.sh

运行：
1. 运行构建步骤生成的bin/sudoku文件
---
操作说明：
- w 光标上移↑
- a 光标左移←
- s 光标下移↓
- d 光标右移→
- 0 删除已填入数字
- u 撤销上一步操作
- enter 尝试通关
- esc 退出游戏
---
项目结构：  
│  .gitignore  
│  build.bat        //Windows一键编译脚本  
│  build.sh         //Linux一键编译脚本  
│  CMakeLists.txt   //CMake项目文件  
│  README.md  
│    
└─src               //源代码目录  
                block.cpp   //数独格子组合类，可代表行、列、九宫格  
                block.h  
                color.h     //颜色类  
                command.cpp //命令类，实现了撤销功能  
                command.h  
                common.h    //公共头文件  
                input.cpp   //输入类  
                input.h  
                main.cpp    //入口文件  
                scene.cpp   //游戏场景类  
                scene.h  
                test.cpp    //测试文件  
                test.h  
                utility.inl //一些实用的全局函数  
