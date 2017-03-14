file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(descriptor_manager SHARED ${SRC})
target_include_directories(descriptor_manager PUBLIC ${Caffe_INCLUDE_DIRS})
target_link_libraries(descriptor_manager ${Caffe_LIBS})