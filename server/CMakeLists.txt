cmake_minimum_required(VERSION 3.16)
project(Server VERSION 1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 确保正确查找Qt Widgets和Network模块
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

add_executable(Server
    main.cpp
    server.cpp
    server.h
)

# 确保正确链接Qt Widgets和Network模块
target_link_libraries(Server PRIVATE
    Qt6::Widgets
    Qt6::Network
)

set_target_properties(Server PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
