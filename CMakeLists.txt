# FILEPATH: c:\Development\University\AI\CMakeLists.txt

cmake_minimum_required(VERSION 3.0)

project(AI2Worksheets)

file(GLOB_RECURSE SOURCES "main.cpp")

file(GLOB_RECURSE WORKSHEETS 
        "/Worksheet 1/Inc/*.h" "/Worksheet 1/*.cpp"
        "/Worksheet 2/Inc/*.h" "/Worksheet 2/*.cpp"
        "/Worksheet 3/Inc/*.h" "/Worksheet 3/*.cpp"
)



add_executable(AI2Worksheets ${SOURCES} ${WORKSHEETS})



target_include_directories(AI2Worksheets PUBLIC
    "/Worksheet 1/Inc/"
    "/Worksheet 2/Inc/"
    "/Worksheet 3/Inc/"
)
