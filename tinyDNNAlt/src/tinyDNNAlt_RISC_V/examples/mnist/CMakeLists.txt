
if(USE_SERIALIZER)
 add_custom_target(mnist_tests_type)
    add_custom_target(mnist_trains_type)    



    add_executable(mnist_train train.cpp ${tiny_dnn_headers})
    target_link_libraries(mnist_train
        ${project_library_target_name} ${REQUIRED_LIBRARIES})
    add_dependencies(mnist_trains_type mnist_train)

    add_executable(mnist_test test_alt.cpp ${tiny_dnn_headers})
    target_link_libraries(mnist_test
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    add_dependencies(mnist_tests_type mnist_test)

    add_executable(mnist_train_p16_0 train.cpp ${tiny_dnn_headers})
    target_link_libraries(mnist_train_p16_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES})
    target_compile_definitions(mnist_train_p16_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=16 CNN_EXP_BITS=0 CNN_POS_STORAGE=int_fast32_t CNN_POS_BACKEND=uint_fast32_t) 
    add_dependencies(mnist_trains_type mnist_train_p16_0)

  

    add_executable(mnist_test_p16_0 test_alt.cpp ${tiny_dnn_headers})
    target_link_libraries(mnist_test_p16_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(mnist_test_p16_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=16 CNN_EXP_BITS=0 CNN_POS_STORAGE=int_fast32_t CNN_POS_BACKEND=uint_fast32_t) 
    add_dependencies(mnist_tests_type mnist_test_p16_0)  

  

    add_executable(mnist_test_p14_0 test_alt.cpp ${tiny_dnn_headers})
    target_link_libraries(mnist_test_p14_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(mnist_test_p14_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=14 CNN_EXP_BITS=0 CNN_POS_STORAGE=int_fast32_t CNN_POS_BACKEND=uint_fast32_t) 
    add_dependencies(mnist_tests_type mnist_test_p14_0)  


  
add_executable(mnist_test_p12_0 test_alt.cpp ${tiny_dnn_headers})
    target_link_libraries(mnist_test_p12_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(mnist_test_p12_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=12 CNN_EXP_BITS=0 CNN_POS_STORAGE=int_fast32_t CNN_POS_BACKEND=uint_fast32_t) 
    add_dependencies(mnist_tests_type mnist_test_p12_0)      

    add_executable(mnist_test_p10_0 test_alt.cpp ${tiny_dnn_headers})
    target_link_libraries(mnist_test_p10_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(mnist_test_p10_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=10 CNN_EXP_BITS=0 CNN_POS_STORAGE=int_fast32_t CNN_POS_BACKEND=uint_fast32_t) 
    add_dependencies(mnist_tests_type mnist_test_p10_0)  

    add_executable(mnist_test_p8_0 test_alt.cpp ${tiny_dnn_headers})
    target_link_libraries(mnist_test_p8_0
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(mnist_test_p8_0 PRIVATE CNN_USE_POSIT CNN_POS_BITS=8 CNN_EXP_BITS=0 CNN_POS_STORAGE=int_fast32_t CNN_POS_BACKEND=uint_fast32_t) 
    add_dependencies(mnist_tests_type mnist_test_p8_0)  

  


    if(TAB8)
        add_executable(mnist_train_posittab8 train.cpp ${TAB_POSIT_LIB8} ${tiny_dnn_headers})
        target_link_libraries(mnist_train_posittab8
            ${project_library_target_name} ${REQUIRED_LIBRARIES})
        target_compile_definitions(mnist_train_posittab8 PRIVATE CNN_USE_POSIT CNN_TAB_TYPE=posit8)
        add_executable(mnist_test_posittab8 test_alt.cpp ${TAB_POSIT_LIB8} ${tiny_dnn_headers})
        target_link_libraries(mnist_test_posittab8
            ${project_library_target_name} ${REQUIRED_LIBRARIES})  
        target_compile_definitions(mnist_test_posittab8 PRIVATE CNN_USE_POSIT CNN_TAB_TYPE=posit8)        
        add_dependencies(mnist_tests_type mnist_test_posittab8)
        add_dependencies(mnist_trains_type mnist_train_posittab8)
    endif(TAB8)

    if(TAB10)
        add_executable(mnist_train_posittab10 train.cpp ${TAB_POSIT_LIB10} ${tiny_dnn_headers})
        target_link_libraries(mnist_train_posittab10
            ${project_library_target_name} ${REQUIRED_LIBRARIES})
        target_compile_definitions(mnist_train_posittab10 PRIVATE CNN_USE_POSIT CNN_TAB_TYPE=posit10)
        add_executable(mnist_test_posittab10 test_alt.cpp ${TAB_POSIT_LIB10} ${tiny_dnn_headers})
        target_link_libraries(mnist_test_posittab10
            ${project_library_target_name} ${REQUIRED_LIBRARIES})  
        target_compile_definitions(mnist_test_posittab10 PRIVATE CNN_USE_POSIT CNN_TAB_TYPE=posit10)        
        add_dependencies(mnist_tests_type mnist_test_posittab10)  
        add_dependencies(mnist_trains_type mnist_train_posittab10)                 
    endif(TAB10)

    if(TAB12)
        add_executable(mnist_train_posittab12 train.cpp ${TAB_POSIT_LIB12} ${tiny_dnn_headers})
        target_link_libraries(mnist_train_posittab12
            ${project_library_target_name} ${REQUIRED_LIBRARIES})
        target_compile_definitions(mnist_train_posittab12 PRIVATE CNN_USE_POSIT CNN_TAB_TYPE=posit12)
        add_executable(mnist_test_posittab12 test_alt.cpp ${TAB_POSIT_LIB12} ${tiny_dnn_headers})
        target_link_libraries(mnist_test_posittab12
            ${project_library_target_name} ${REQUIRED_LIBRARIES})  
        target_compile_definitions(mnist_test_posittab12 PRIVATE CNN_USE_POSIT CNN_TAB_TYPE=posit12)        
        add_dependencies(mnist_tests_type mnist_test_posittab12)     
        add_dependencies(mnist_trains_type mnist_train_posittab12)                           
    endif(TAB12)
    
    
    if(TAB14)
        add_executable(mnist_train_posittab14 train.cpp ${TAB_POSIT_LIB14} ${tiny_dnn_headers})
        target_link_libraries(mnist_train_posittab14
            ${project_library_target_name} ${REQUIRED_LIBRARIES})
        target_compile_definitions(mnist_train_posittab14 PRIVATE CNN_USE_POSIT CNN_TAB_TYPE=posit14)
        add_executable(mnist_test_posittab14 test_alt.cpp ${TAB_POSIT_LIB14} ${tiny_dnn_headers})
        target_link_libraries(mnist_test_posittab14
            ${project_library_target_name} ${REQUIRED_LIBRARIES})  
        target_compile_definitions(mnist_test_posittab14 PRIVATE CNN_USE_POSIT CNN_TAB_TYPE=posit14)        
        add_dependencies(mnist_tests_type mnist_test_posittab14)   
        add_dependencies(mnist_trains_type mnist_train_posittab14)                                      
    endif(TAB14)
endif()