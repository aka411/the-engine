#unit/CMakeLists.txt



include(GoogleTest)


add_executable(SanityCheckTests sanity_check.cpp)



target_link_libraries(SanityCheckTests PRIVATE
    GTest::gtest_main
)









gtest_discover_tests(SanityCheckTests)


