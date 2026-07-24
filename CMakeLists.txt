cmake_minimum_required(VERSION 3.10)
project(MySchedule VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

add_subdirectory(src/core)
add_subdirectory(src/cli)

find_package(Qt5 COMPONENTS Widgets Multimedia)
if(Qt5_FOUND)
    add_subdirectory(src/gui)
    message(STATUS "✅ Qt GUI support enabled")
else()
    message(STATUS "⚠️ Qt not found, GUI will be skipped")
endif()
