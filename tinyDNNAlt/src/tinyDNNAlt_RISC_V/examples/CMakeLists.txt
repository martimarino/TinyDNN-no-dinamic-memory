

if(MSVC)
    file(GLOB_RECURSE tiny_dnn_headers "${CMAKE_SOURCE_DIR}/tiny_dnn/*.h")
    file(RELATIVE_PATH tiny_dnn_relpath ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/tiny_dnn")
    string(REPLACE "/" "\\" tiny_dnn_relpath "${tiny_dnn_relpath}")
    foreach(_source IN ITEMS ${tiny_dnn_headers})
        file(RELATIVE_PATH _source_rel ${CMAKE_CURRENT_SOURCE_DIR} "${_source}")
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        string(REPLACE ${tiny_dnn_relpath} "tiny_dnn" _source_path_msvc ${_source_path_msvc})
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endif()
#add_executable(benchmarks_all benchmarks/main.cpp ${tiny_dnn_headers} )
#target_link_libraries(benchmarks_all
#    ${project_library_target_name} ${REQUIRED_LIBRARIES})
#cotire(benchmarks_all)

message(LOG "Loading tab lib ${CPPOSIT_SRC_DIR}")
list(APPEND TAB_POSIT_LIB8 ${CPPOSIT_SRC_DIR}/posit8_tbl.cpp ${CPPOSIT_SRC_DIR}/posit8.cpp)
list(APPEND TAB_POSIT_LIB10 ${CPPOSIT_SRC_DIR}/posit10_tbl.cpp ${CPPOSIT_SRC_DIR}/posit10.cpp)

if(USE_SERIALIZER)
    add_subdirectory(${CMAKE_SOURCE_DIR}/examples/cifar10)
    add_subdirectory(${CMAKE_SOURCE_DIR}/examples/GTRSB)
    add_subdirectory(${CMAKE_SOURCE_DIR}/examples/mnist)
    add_subdirectory(${CMAKE_SOURCE_DIR}/examples/ImageNet)
    if(USE_SVE)
        add_subdirectory(${CMAKE_SOURCE_DIR}/examples/sve_test)

    endif(USE_SVE)
endif()

    add_subdirectory(${CMAKE_SOURCE_DIR}/examples/qttest)

if(PROTO_CPP_AVAILABLE)
    include_directories(${PROTOBUF_INCLUDE_DIRS})
    set_source_files_properties(${proto_file} PROPERTIES GENERATED TRUE)
    add_executable(example_caffe_converter
                   caffe_converter/caffe_converter.cpp ${proto_file} ${tiny_dnn_headers})
    target_link_libraries(example_caffe_converter
            ${project_library_target_name}
            ${PROTOBUF_LIBRARIES} ${REQUIRED_LIBRARIES})
    cotire(example_caffe_converter)
endif()

if(PROTO_CPP_GENERATE)
    add_dependencies(example_caffe_converter generated_proto)
endif()
#
#if(USE_CBLAS)
#    add_executable(example_cblas_mlp backends/cblas/test_mlp.cpp ${tiny_dnn_headers})
#    target_link_libraries(example_cblas_mlp
#            ${project_library_target_name} ${REQUIRED_LIBRARIES})
#endif()
#
