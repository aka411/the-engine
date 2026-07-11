


add_executable(compile-test 
"compile_test.cpp"
)


target_link_libraries(compile-test  PRIVATE

    TheEngine
)