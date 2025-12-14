cmake_minimum_required(VERSION 3.15)
project(mystl)
set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include_directories(${PROJECT_SOURCE_DIR}/../include)

add_executable(APP ${PROJECT_SOURCE_DIR}/src/list_text.cpp)
add_executable(DEQUE_APP ${PROJECT_SOURCE_DIR}/src/deque_text.cpp)
set_target_properties(DEQUE_APP PROPERTIES CXX_STANDARD 98 CXX_STANDARD_REQUIRED ON)



