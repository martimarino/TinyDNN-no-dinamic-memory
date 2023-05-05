
if(USE_SERIALIZER)
    add_custom_target(gtrsb_tests_type)
    add_custom_target(gtrsb_trains_type)    



    add_executable(gtrsb_train train.cpp ${tiny_dnn_headers})
    target_link_libraries(gtrsb_train
        ${project_library_target_name} ${REQUIRED_LIBRARIES})
    add_dependencies(gtrsb_trains_type gtrsb_train)

    add_executable(gtrsb_test test.cpp ${tiny_dnn_headers})
    target_link_libraries(gtrsb_test
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    add_dependencies(gtrsb_tests_type gtrsb_test)


    add_executable(gtrsb_test_p16_0 test.cpp ${tiny_dnn_headers})
    target_link_libraries(gtrsb_test_p16_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(gtrsb_test_p16_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=16 CNN_EXP_BITS=0 CNN_POS_STORAGE=int16_t CNN_POS_BACKEND=uint_fast32_t) 
    add_dependencies(gtrsb_tests_type gtrsb_test_p16_0)  

    add_executable(gtrsb_test_p16_1 test.cpp ${tiny_dnn_headers})
    target_link_libraries(gtrsb_test_p16_1
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(gtrsb_test_p16_1 PRIVATE CNN_USE_POSIT CNN_POS_BITS=16 CNN_EXP_BITS=1 CNN_POS_STORAGE=int16_t CNN_POS_BACKEND=uint_fast32_t) 
    add_dependencies(gtrsb_tests_type gtrsb_test_p16_1)      

  
    add_executable(gtrsb_test_p14_0 test.cpp ${tiny_dnn_headers})
    target_link_libraries(gtrsb_test_p14_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(gtrsb_test_p14_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=14 CNN_EXP_BITS=0 CNN_POS_STORAGE=int16_t CNN_POS_BACKEND=uint_fast32_t) 
    add_dependencies(gtrsb_tests_type gtrsb_test_p14_0)  


    
add_executable(gtrsb_test_p12_0 test.cpp ${tiny_dnn_headers})
    target_link_libraries(gtrsb_test_p12_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(gtrsb_test_p12_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=12 CNN_EXP_BITS=0 CNN_POS_STORAGE=int16_t CNN_POS_BACKEND=uint_fast32_t) 
    add_dependencies(gtrsb_tests_type gtrsb_test_p12_0)      

    add_executable(gtrsb_test_p10_0 test.cpp ${tiny_dnn_headers})
    target_link_libraries(gtrsb_test_p10_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(gtrsb_test_p10_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=10 CNN_EXP_BITS=0 CNN_POS_STORAGE=int16_t CNN_POS_BACKEND=uint_fast32_t) 
    add_dependencies(gtrsb_tests_type gtrsb_test_p10_0)  

    add_executable(gtrsb_test_p8_0 test.cpp ${tiny_dnn_headers})
    target_link_libraries(gtrsb_test_p8_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(gtrsb_test_p8_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=8 CNN_EXP_BITS=0 CNN_POS_STORAGE=int8_t CNN_POS_BACKEND=uint_fast32_t)
    add_dependencies(gtrsb_tests_type gtrsb_test_p8_0)    

    add_executable(gtrsb_test_b8 test.cpp ${tiny_dnn_headers})
    target_link_libraries(gtrsb_test_b8
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(gtrsb_test_b8 PRIVATE CNN_USE_BFLOAT8) 
    add_dependencies(gtrsb_tests_type gtrsb_test_b8)  

    add_executable(gtrsb_test_b16 test.cpp ${tiny_dnn_headers})
    target_link_libraries(gtrsb_test_b16
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(gtrsb_test_b16 PRIVATE CNN_USE_BFLOAT16) 
    add_dependencies(gtrsb_tests_type gtrsb_test_b16)  
   
endif()