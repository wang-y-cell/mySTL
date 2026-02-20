cmake_minimum_required(VERSION 3.15) #指定最低Cmake版本
project(mystl VERSION 1.1)           #定义项目名称和版本

# 设置c++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED NO)

# 创建一个INTERFACE库（Header-only）
add_library(mystl INTERFACE)

# 设置头文件包含路径
target_include_directories(mystl INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 可选：安装规则
include(GNUInstallDirs)
install(TARGETS mystl
    EXPORT mystlTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 添加子目录
add_subdirectory(tests)
