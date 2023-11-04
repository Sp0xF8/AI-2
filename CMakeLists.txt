# FILEPATH: c:\Development\University\AI\CMakeLists.txt

cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)

project(AI2Worksheets)

file(GLOB_RECURSE SOURCES "Dev/main.cpp")

file(GLOB_RECURSE Assignment "Dev/Assignment/*.cpp" "Dev/Assignment/*.h")






add_executable(AI2Worksheets ${SOURCES} ${Assignment})



target_include_directories(AI2Worksheets PUBLIC
    "Dev/Assignment/"

)
