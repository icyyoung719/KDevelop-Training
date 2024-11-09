cmake_minimum_required(VERSION 3.10)

project(CMakeTest)

# 获取所有头文件
file(GLOB_RECURSE header_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
source_group("Header Files" FILES ${header_files})

# 获取当前目录下的所有源文件
aux_source_directory(. SRCS)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SRCS} ${header_files})