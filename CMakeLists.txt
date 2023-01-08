cmake_minimum_required(VERSION 3.0)

project(robopiddemo)

add_executable(robopiddemo main.cpp)

install(TARGETS robopiddemo RUNTIME DESTINATION bin)
