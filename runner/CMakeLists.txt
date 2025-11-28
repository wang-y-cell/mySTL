cmake_minimum_required(VERSION 3.15)
project(mystl)
# c++版本设置
set(CMAKE_CXX_STANDARD 11) 


# 可执行文件位置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
aux_source_directory(${PROJECT_SOURCE_DIR}/src RCS)
include_directories(${PROJECT_SOURCE_DIR}/../include)

add_executable(APP ${RCS})



