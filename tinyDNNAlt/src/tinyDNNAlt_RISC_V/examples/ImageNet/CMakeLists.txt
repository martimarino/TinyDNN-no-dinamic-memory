
if(USE_SERIALIZER)
    add_custom_target(inet_tests_type)
    add_custom_target(inet_trains_type)    


    add_executable(inet_test test.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_test
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    add_dependencies(inet_tests_type inet_test)      


  add_executable(inet_test_p16_0_RVV test.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_test_p16_0_RVV
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(inet_test_p16_0_RVV PRIVATE CNN_USE_POSIT CNN_USE_RVV CNN_POS_BITS=16 CNN_EXP_BITS=0 CNN_POS_STORAGE=int16_t CNN_POS_BACKEND=uint16_t) 
    add_dependencies(inet_tests_type inet_test_p16_0_RVV)      

  add_executable(inet_test_p8_0_RVV test.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_test_p8_0_RVV
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(inet_test_p8_0_RVV PRIVATE CNN_USE_POSIT CNN_USE_RVV CNN_POS_BITS=8 CNN_EXP_BITS=0 CNN_POS_STORAGE=int8_t CNN_POS_BACKEND=uint8_t) 
    add_dependencies(inet_tests_type inet_test_p8_0_RVV)              
  
  add_executable(inet_test_p16_0_n test.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_test_p16_0_n
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(inet_test_p16_0_n PRIVATE CNN_USE_POSIT CNN_POS_BITS=16 CNN_EXP_BITS=0 CNN_POS_STORAGE=int16_t CNN_POS_BACKEND=uint16_t) 
    add_dependencies(inet_tests_type inet_test_p16_0_n) 

  add_executable(inet_test_p16_1_n test.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_test_p16_1_n
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(inet_test_p16_1_n PRIVATE CNN_USE_POSIT CNN_POS_BITS=16 CNN_EXP_BITS=1 CNN_POS_STORAGE=int16_t CNN_POS_BACKEND=uint16_t) 
    add_dependencies(inet_tests_type inet_test_p16_1_n) 

  add_executable(inet_test_p8_0_n test.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_test_p8_0_n
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(inet_test_p8_0_n PRIVATE CNN_USE_POSIT CNN_POS_BITS=8 CNN_EXP_BITS=0 CNN_POS_STORAGE=int8_t CNN_POS_BACKEND=uint8_t) 
    add_dependencies(inet_tests_type inet_test_p8_0_n) 

   add_executable(inet_test_p8_0_p test.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_test_p8_0_p
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(inet_test_p8_0_p PRIVATE RISCV_PPU CNN_USE_POSIT CNN_POS_BITS=8 CNN_EXP_BITS=0 CNN_POS_STORAGE=int8_t CNN_POS_BACKEND=uint8_t) 
    add_dependencies(inet_tests_type inet_test_p8_0_p)

 add_executable(inet_test_p16_0_p test.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_test_p16_0_p
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(inet_test_p16_0_p PRIVATE RISCV_PPU CNN_USE_POSIT CNN_POS_BITS=16 CNN_EXP_BITS=0 CNN_POS_STORAGE=int16_t CNN_POS_BACKEND=uint16_t) 
    add_dependencies(inet_tests_type inet_test_p16_0_p)


 add_executable(inet_test_p16_1_p test.cpp ${tiny_dnn_headers})
    target_link_libraries(inet_test_p16_1_p
        ${project_library_target_name} ${REQUIRED_LIBRARIES}) 
    target_compile_definitions(inet_test_p16_1_p PRIVATE RISCV_PPU CNN_USE_POSIT CNN_POS_BITS=16 CNN_EXP_BITS=1 CNN_POS_STORAGE=int16_t CNN_POS_BACKEND=uint16_t) 
    add_dependencies(inet_tests_type inet_test_p16_1_p)



endif()
