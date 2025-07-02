
cmake_minimum_required(VERSION 3.14 ) # FetchContent_MakeAvailable requires 3.14+

# Include the FetchContent module
include(FetchContent)

# Declare Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.14.0 # Use a specific version for stability

)

# Populate Google Test (download and make its targets available)
FetchContent_MakeAvailable(googletest)

# Now, Google Test targets like GTest::gtest_main are available.

#  subdirectory for unit tests
add_subdirectory(unit)

#  subdirectory for integration tests
add_subdirectory(integration)