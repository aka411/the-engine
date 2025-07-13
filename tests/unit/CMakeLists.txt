#unit/CMakeLists.txt



# --- Sanity Check Test Executable ---
# Defines an executable named 'SanityCheckTests' that will contain basic tests.
add_executable(SanityCheckTests sanity_check.cpp)

add_executable(CPUResourceManagerTests 
    resource/cpu_resource_manager_test.cpp
)
# Links the test executable to Google Test's main library.
# 'GTest::gtest_main' provides the main() function needed to run Google Tests.
target_link_libraries(SanityCheckTests PRIVATE
    GTest::gtest_main
)
# Link it against GTest (which provides the main() function for tests)
target_link_libraries(CPUResourceManagerTests  PRIVATE
GTest::gtest_main
TheEngine_Resource
)

# Includes CMake's GoogleTest module. This provides the `gtest_discover_tests` function.
include(GoogleTest)

# Automatically discovers all TEST() and TEST_F() macros within SanityCheckTests
# and registers them with CTest, allowing you to run them with `ctest`.

gtest_discover_tests(SanityCheckTests)

gtest_discover_tests(CPUResourceManagerTests  )


add_executable(GPUResourceManagerTests
   graphics/gpu_resource_manager_test.cpp
)

target_link_libraries(
    GPUResourceManagerTests PRIVATE
    GTest::gtest_main
    TheEngine_Graphics
)

gtest_discover_tests(GPUResourceManagerTests)