unset(CAFFE_FOUND)

find_path(Caffe_INCLUDE_DIRS NAMES caffe/caffe.hpp caffe/common.hpp caffe/net.hpp caffe/proto/caffe.pb.h caffe/util/io.hpp
        HINTS
        /Users/calvarez/Git/caffe/include
        /home/calvarez/dependencies/caffe/include)
find_library(Caffe_LIBS REQUIRED NAMES caffe
        PATHS
        /Users/calvarez/Git/caffe/build/lib
        /home/calvarez/dependencies/caffe/lib)

if(Caffe_LIBS AND Caffe_INCLUDE_DIR)
    set(Caffe_FOUND 1)
endif()