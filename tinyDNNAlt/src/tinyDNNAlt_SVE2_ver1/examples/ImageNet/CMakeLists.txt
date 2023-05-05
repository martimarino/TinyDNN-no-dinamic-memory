
if(USE_SERIALIZER)
    add_custom_target(inet_tests_type)
    add_custom_target(inet_trains_type)    



    add_executable(inet_train train.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_train
        ${project_library_target_name} ${REQUIRED_LIBRARIES})
    add_dependencies(inet_trains_type inet_train)

    add_executable(inet_test test.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_test
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    add_dependencies(inet_tests_type inet_test)

    add_executable(inet_test_p16_0 test.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_test_p16_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(inet_test_p16_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=16 CNN_EXP_BITS=0 CNN_POS_STORAGE=int16_t CNN_POS_BACKEND=uint32_t) 
    add_dependencies(inet_tests_type inet_test_p16_0)  

    add_executable(inet_test_p10_0 test.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_test_p10_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(inet_test_p10_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=10 CNN_EXP_BITS=0 CNN_POS_STORAGE=int16_t CNN_POS_BACKEND=uint32_t) 
    add_dependencies(inet_tests_type inet_test_p10_0)  

    add_executable(inet_test_p8_0 test.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_test_p8_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(inet_test_p8_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=8 CNN_EXP_BITS=0 CNN_POS_STORAGE=int8_t CNN_POS_BACKEND=uint32_t) 
    add_dependencies(inet_tests_type inet_test_p8_0)  

 
endif()
