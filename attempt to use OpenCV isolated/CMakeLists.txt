cmake_minimum_required(VERSION 3.19)
set(CONAN_DISABLE_CHECK_COMPILER TRUE)
set(PROJECT_NAME GaussianBlur)
project(${PROJECT_NAME})

enable_language(C)
enable_language(CXX)

set(CMAKE_DEBUG_POSTFIX _d)
set(CMAKE_BUILD_TYPE Debug)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

if(APPLE)
	set (CMAKE_OSX_DEPLOYMENT_TARGET "10.10")
endif()
 
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(SOURCE_FILES
	src/BlurLib.cpp
	src/BlurLib.h
	src/Platform.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})