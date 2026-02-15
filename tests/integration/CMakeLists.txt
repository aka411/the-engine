#integration/CMakeLists.txt



include(GoogleTest)


add_executable(place-holder-test

)



target_link_libraries(place-holder-test PRIVATE
    GTest::gtest_main
    GTest::gmock_main

)


gtest_discover_tests(place-holder-test)

