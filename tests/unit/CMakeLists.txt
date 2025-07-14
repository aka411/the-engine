#unit/CMakeLists.txt


# Includes CMake's GoogleTest module. This provides the `gtest_discover_tests` function.
include(GoogleTest)

# --- Sanity Check Test Executable ---
# Defines an executable named 'SanityCheckTests' that will contain basic tests.
add_executable(SanityCheckTests sanity_check.cpp)


# Links the test executable to Google Test's main library.
# 'GTest::gtest_main' provides the main() function needed to run Google Tests.
target_link_libraries(SanityCheckTests PRIVATE
    GTest::gtest_main
)


add_executable(CPUResourceManagerTests 
    resource/cpu_resource_manager_test.cpp
)

# Link it against GTest (which provides the main() function for tests)
target_link_libraries(CPUResourceManagerTests  PRIVATE
GTest::gtest_main
TheEngine_Resource
)


add_executable(GPUResourceSystemTests
   graphics/gpu_resource_manager_test.cpp
   graphics/gpu_resource_system_test.cpp
)

target_link_libraries(
    GPUResourceSystemTests PRIVATE
    GTest::gtest_main
    GTest::gmock_main
    TheEngine_Graphics
    TheEngine_Resource
    TheEngine_Core
)











# Automatically discovers all TEST() and TEST_F() macros within SanityCheckTests
# and registers them with CTest, allowing you to run them with `ctest`.

gtest_discover_tests(SanityCheckTests)

gtest_discover_tests(CPUResourceManagerTests )

gtest_discover_tests(GPUResourceSystemTests)

