cmake_minimum_required(VERSION 3.25)

project(Junius7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS OFF)  # this will force SFML to build static libraries

# use FetchContent to obtain SFML source
include(FetchContent)
FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML
        GIT_TAG 11b73743c42cf7ecd7c596ba83fdbf1150ffa96c # release-2.6.0
)

FetchContent_MakeAvailable(sfml)

add_executable(Junius7 src/Input.cpp src/SnakeSection.cpp src/apple.cpp src/apple.h)

target_include_directories(Junius7 PUBLIC headers)

target_link_libraries(Junius7 PRIVATE sfml-graphics)

target_sources(Junius7 PRIVATE
        src/main.cpp
        src/Engine.cpp
        src/draw.cpp
        src/input.cpp
        )

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    target_compile_options(Junius7 PRIVATE -Wall -Wextra -Wpedantic)
endif()