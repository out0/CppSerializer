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

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# # framework lib
# find_library(DATALINK_LIB REQUIRED
#     NAMES datalink
#     HINTS /usr/local/lib
# )

SET(TARGET_H
  ../include/serializer.h
)


add_executable(
  unittest
    cpp_test_base.cpp
    test_raw_data.cpp
    test_basic.cpp
    test_array.cpp
    test_string.cpp
    ../src/serializer.cpp
)


if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(unittest PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(unittest "${CMAKE_THREAD_LIBS_INIT}")
endif()

# link my-recognition to jetson-inference library
target_link_libraries(unittest PkgConfig::deps 
  ${GTEST_LIBRARIES} 
  #${DATALINK_LIB}
)

