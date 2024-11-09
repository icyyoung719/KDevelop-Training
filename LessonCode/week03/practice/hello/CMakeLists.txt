cmake_minimum_required(VERSION 3.10)

project(factorialtest)



add_executable(${PROJECT_NAME})

find_package(GTest CONFIG REQUIRED)

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
)


#链接需要操作的库
target_link_libraries(${PROJECT_NAME} PRIVATE 
    GTest::gtest
    GTest::gmock
)