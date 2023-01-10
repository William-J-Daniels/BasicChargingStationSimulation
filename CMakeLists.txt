cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

project(robopiddemo VERSION 0.2.0)

add_library(${PROJECT_NAME} STATIC
    include/robot.h
    include/pidcontroller.h
    src/robot.cpp
    src/pidcontroller.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(public_headers
    include/robot.h
    include/pidcontroller.h
)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")

add_executable(exe main.cpp)
target_link_libraries(exe PUBLIC ${PROJECT_NAME})
