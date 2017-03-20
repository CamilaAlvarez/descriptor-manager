find_package(Threads REQUIRED)
include(ExternalProject)
ExternalProject_Add(Caffe
        URL https://github.com/BVLC/caffe/archive/rc5.zip
        CMAKE_ARGS -DCPU_ONLY=${CAFFE_MODE} -DOpenCV_DIR=${OPENCV_CMAKE_DIR}
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/caffe
        BINARY_DIR ${CMAKE_SOURCE_DIR}/caffe)

find_package(Caffe REQUIRED HINTS ${CMAKE_SOURCE_DIR}/caffe)

