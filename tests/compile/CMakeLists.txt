


if(THE_ENGINE_ANDROID_TEST)


    add_subdirectory(android)


else()
add_executable(compile-test 
"compile_test.cpp"
)


target_link_libraries(compile-test  PRIVATE
    TheEngine
)
endif()