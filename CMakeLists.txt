cmake_minimum_required(VERSION 3.15)
project(c)

set (CMAKE_CXX_STANDARD 11)
  
set(CMAKE_TOOLCHAIN_FILE "/home/sid/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set(CMAKE_CXX_FLAGS "-g -O0")

# Ensure debug symbols are generated
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
set(BUILD_SHARED_LIBS OFF)

file(GLOB SOURCES "src/*.cpp")

add_executable(FoxyRaylibSandbox WIN32 ${SOURCES})

find_package(raylib CONFIG REQUIRED)
target_link_libraries(FoxyRaylibSandbox PRIVATE raylib)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(FoxyRaylibSandbox PRIVATE glfw)
if(WIN32) 
  target_link_libraries(FoxyRaylibSandbox PRIVATE winmm)

  target_link_options(FoxyRaylibSandbox PRIVATE
    -static
    -static-libgcc
    -static-libstdc++
  ) 
endif()
