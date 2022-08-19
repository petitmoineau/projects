cmake_minimum_required(VERSION 3.19)
set(CONAN_DISABLE_CHECK_COMPILER TRUE)
set(PROJECT_NAME EmotionRecognizing)
project(${PROJECT_NAME})

enable_language(C)
enable_language(CXX)

set(CMAKE_DEBUG_POSTFIX _d)
set(CMAKE_BUILD_TYPE Debug)
set (CMAKE_CXX_STANDARD 14)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

if(APPLE)
	set (CMAKE_OSX_DEPLOYMENT_TARGET "11.3")
endif()
 
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(SOURCE_FILES
	src/EmotionDetection.h
	src/EmotionDetection.cpp
	src/EmotionAnalyzer.cpp
	src/EmotionAnalyzer.h
	src/UserStateObserver.h
	src/ActReactTranslator.cpp
	src/ActReactTranslator.h
	src/Platform.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})