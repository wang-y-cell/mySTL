cmake_minimum_required(VERSION 3.15) #指定最低Cmake版本
project(mystl VERSION 1.1)           #定义项目名称和版本

# 设置c++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED NO)

# 包含头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加子目录
add_subdirectory(tests)