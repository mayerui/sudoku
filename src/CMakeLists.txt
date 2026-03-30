# src目录的CMakeLists.txt文件

# 手动指定源文件
set(SOURCE_FILES
    block.cpp
    command.cpp
    i18n.cpp
    input.cpp
    scene.cpp
    main.cpp
)

add_executable(sudoku  # 输出名为sudoku的可执行文件
   ${SOURCE_FILES}
)
