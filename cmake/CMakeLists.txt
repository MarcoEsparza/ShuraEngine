cmake_minimum_requiered(VERSION 3.24.2)

project(ShuraEngine LANGUAGES C++)

add_executable(${PROJECT_SOURCE_DIR}/shUtilitiesTest/shUtilitiesTestMain.cpp)

target_include_directories(ShuraEngine)