cmake_minimum_required(VERSION 3.10)
project(release)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)  # Ensures Debug mode
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")  # Add debug symbols and disable optimizations

set(GLFW_BIN ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw-3.4.bin.MACOS/lib-arm64/libglfw3.a)
set(GLEW_BIN ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew-2.2.0/lib/libGLEW.a)
set(FREETYPE_BIN ${CMAKE_CURRENT_SOURCE_DIR}/vendor/freetype-2.13.3/build/libfreetype.a)
set(LOCAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${LOCAL_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw-3.4.bin.MACOS/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew-2.2.0/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/freetype-2.13.3/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor)

set(PNG_SUPPORT OFF CACHE BOOL "Disable PNG support in FreeType")

file(GLOB_RECURSE SOURCES "src/*.cpp" "application/*.cpp")
add_executable(release ${SOURCES} "CocoaHelper.mm" "main.cpp" )


target_link_libraries(release ${GLFW_BIN} ${GLEW_BIN} ${FREETYPE_BIN}
    "-framework Cocoa"
    "-framework OpenGL"
    "-framework IOKit"
    "-framework CoreVideo"
    "-framework QuartzCore"
    )