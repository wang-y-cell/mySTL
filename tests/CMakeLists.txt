# 解决 Windows 下中文乱码问题
# 源文件使用 UTF-8，执行文件输出使用 GBK (适配默认 Windows 终端)
if(MSVC)
    add_compile_options(/source-charset:utf-8 /execution-charset:gbk)
elseif(MINGW)
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=GBK)
    # 静态链接标准库，方便分发 exe
    add_link_options(-static)
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)
foreach(src ${SRC})
    get_filename_component(name ${src} NAME_WE)
    add_executable(${name} ${src})
endforeach()




