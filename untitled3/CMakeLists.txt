cmake_minimum_required(VERSION 3.13)
project(untitled3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-lmenu -lform -lncurses -g")
add_executable(untitled3 main.cpp Task.cpp Task.h TaskList.cpp TaskList.h)