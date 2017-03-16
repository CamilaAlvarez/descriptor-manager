find_package(Threads REQUIRED)
include(ExternalProject)
ExternalProject_Add(Caffe
        URL https://github.com/BVLC/caffe/archive/rc5.zip
        CMAKE_ARGS -DCPU_ONLY=ON
        INSTALL_COMMAND "")

ExternalProject_Get_Property(Caffe BINARY_DIR)

find_package(Caffe REQUIRED)
