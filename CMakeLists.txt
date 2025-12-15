# require CMake 2.8 or greater
cmake_minimum_required(VERSION 3.11)

project(unittest)
include(GNUInstallDirs)
include_directories(/usr/include)

find_package(Threads REQUIRED) 


set(CMAKE_CXX_STANDARD 17)
include_directories(${PROJECT_SOURCE_DIR})

find_package(PkgConfig REQUIRED)


pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0)
pkg_search_module(GLIB REQUIRED glib-2.0) 


add_executable(
  cpps
    cpp_serialize.cpp
)

