# tests目录的CMakeLists.txt文件

# 手动指定源文件
set(PERFORMANCE_TEST_FILES
    ../src/block.cpp
    ../src/command.cpp
    ../src/i18n.cpp
    ../src/input.cpp
    ../src/scene.cpp
    performance_test.cpp
)

add_executable(performance_test  # 输出名为performance_test的可执行文件
   ${PERFORMANCE_TEST_FILES}
)
