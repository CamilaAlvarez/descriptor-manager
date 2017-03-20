find_package(Threads REQUIRED)
include(ExternalProject)
ExternalProject_Add(Caffe
        URL https://github.com/BVLC/caffe/archive/rc5.zip
        CMAKE_ARGS -DCPU_ONLY=ON -DOpenCV_DIR=/home/calvarez/source/OrandLibs/linux_64bits/opencv-3.1.0/build/
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/caffe
        BINARY_DIR ${CMAKE_SOURCE_DIR}/caffe)

find_package(Caffe REQUIRED HINTS ${CMAKE_SOURCE_DIR}/caffe)

