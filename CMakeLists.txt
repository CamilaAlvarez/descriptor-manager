cmake_minimum_required(VERSION 3.5)
project(descriptor_manager CXX)


find_package(OpenCV HINTS /home/calvarez/dependencies)
include(cmake/FindCaffe.cmake)
include_directories("${CMAKE_SOURCE_DIR}/include")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_subdirectory(src)