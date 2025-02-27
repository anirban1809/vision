cmake_minimum_required(VERSION 3.10)
project(release)

set(CMAKE_CXX_STANDARD 17)

set(LOCAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${LOCAL_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(release ${SOURCES} "main.cpp")

target_link_libraries(release)