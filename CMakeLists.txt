cmake_minimum_required(VERSION 3.10)
project(MySchedule VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)  # Qt 需要

# 包含子目录（后续逐步添加）
add_subdirectory(src/core)
add_subdirectory(src/cli)
# add_subdirectory(src/gui)  # 等实现图形界面时再取消注释
