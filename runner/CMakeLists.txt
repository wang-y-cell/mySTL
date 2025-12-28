cmake_minimum_required(VERSION 3.15)
project(mystl)
set(CMAKE_CXX_STANDARD 11)



set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include_directories(${PROJECT_SOURCE_DIR}/../include)

file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)
foreach(src ${SRC})
    get_filename_component(name ${src} NAME_WE)
    add_executable(${name} ${src})
endforeach()




