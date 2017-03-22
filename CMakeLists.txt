cmake_minimum_required(VERSION 3.5)
project(descriptor_manager CXX)

set(USE_LOG TRUE CACHE BOOL "ON/TRUE if google logging is enabled")

find_package(OpenCV REQUIRED HINTS /home/calvarez/source/OrandLibs/linux_64bits/opencv-3.1.0/build/ )
set(CAFFE_CMAKE_DIR /home/calvarez/source/descriptor-manager/caffe/ CACHE STRING "CAFFE BUILD DIR")
find_package(Caffe REQUIRED HINTS ${CAFFE_CMAKE_DIR})
include(cmake/FindGlog.cmake)
include(cmake/FindGFlags.cmake)
include_directories("${CMAKE_SOURCE_DIR}/include")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_subdirectory(src)
add_subdirectory(tools)