cmake_minimum_required(VERSION 3.19)
project(task_6 VERSION 1.0  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)


file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE HEADER_FILES 
    ${CMAKE_SOURCE_DIR}/src/*.h)

add_executable(Task_6 main.cpp ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(Task_6 PRIVATE ${CMAKE_SOURCE_DIR}/src/)