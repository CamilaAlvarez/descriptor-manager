file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(descriptor_manager SHARED ${SRC})

if (GLOG_FOUND)
    target_include_directories(descriptor_manager PUBLIC ${GLOG_INCLUDE_DIRS})
    target_link_libraries(descriptor_manager ${GLOG_LIBRARIES})
    target_compile_definitions(descriptor_manager PRIVATE HAS_GLOG)
    if (USE_LOG)
        target_compile_definitions(descriptor_manager PRIVATE HAS_LOG)
    endif ()
endif ()

target_include_directories(descriptor_manager PUBLIC ${Caffe_INCLUDE_DIRS})
target_link_libraries(descriptor_manager ${Caffe_LIBRARIES})
target_compile_definitions(descriptor_manager PUBLIC ${Caffe_DEFINITIONS})


