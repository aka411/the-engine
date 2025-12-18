#integration/CMakeLists.txt



include(GoogleTest)


add_executable(ecs-integration-test
ecs/ecs_engine_test.cpp
)



target_link_libraries(ecs-integration-test PRIVATE
    GTest::gtest_main
    GTest::gmock_main
    TheEngine_ECS
    TheEngine_Utils
)


gtest_discover_tests(ecs-integration-test)

