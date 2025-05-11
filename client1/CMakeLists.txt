cmake_minimum_required(VERSION 3.16)
project(client1 VERSION 1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
if(WIN32)
    set(PROJECT_SOURCES ${PROJECT_SOURCES} appicon.rc)
endif()
add_executable(client1
    main.cpp
    client.cpp
    client.h
    login.cpp
    login.h
)

target_link_libraries(client1 PRIVATE
    Qt6::Widgets
    Qt6::Network
)

set_target_properties(client1 PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
