find_package(Boost COMPONENTS system filesystem REQUIRED)
add_executable(run_retrieval_experiment ${CMAKE_CURRENT_SOURCE_DIR}/src/run_retrieval_experiment.cpp)
add_executable(compute_descriptors ${CMAKE_CURRENT_SOURCE_DIR}/src/compute_descriptors.cpp)
add_executable(run_retrieval_experiment_with_descriptors ${CMAKE_CURRENT_SOURCE_DIR}/src/run_retrieval_experiments_with_descriptors.cpp)
add_executable(change_class_in_descriptor_file ${CMAKE_CURRENT_SOURCE_DIR}/src/change_class_in_descriptor_file.cpp)

add_dependencies(run_retrieval_experiment descriptor_manager)
add_dependencies(compute_descriptors descriptor_manager)
add_dependencies(run_retrieval_experiment_with_descriptors descriptor_manager)
add_dependencies(change_class_in_descriptor_file descriptor_manager)

if (GFLAGS_NOT_FOUND)
    message(FATAL_ERROR "GFLAGS IS REQUIRED")
endif ()
if (GLOG_NOT_FOUND)
    message(FATAL_ERROR "GLOG IS REQUIRED")
endif ()
target_include_directories(run_retrieval_experiment PRIVATE ${GFLAGS_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})
target_link_libraries(run_retrieval_experiment ${GFLAGS_LIBRARIES} ${Boost_LIBRARIES} descriptor_manager )

target_include_directories(run_retrieval_experiment_with_descriptors PRIVATE ${GFLAGS_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})
target_link_libraries(run_retrieval_experiment_with_descriptors ${GFLAGS_LIBRARIES} ${Boost_LIBRARIES} descriptor_manager )

target_include_directories(compute_descriptors PRIVATE ${GFLAGS_INCLUDE_DIRS} )
target_link_libraries(compute_descriptors ${GFLAGS_LIBRARIES} descriptor_manager )

target_include_directories(change_class_in_descriptor_file PRIVATE ${GFLAGS_INCLUDE_DIRS} ${GLOG_INCLUDE_DIRS} )
target_link_libraries(change_class_in_descriptor_file ${GFLAGS_LIBRARIES} ${GLOG_LIBRARIES} descriptor_manager )