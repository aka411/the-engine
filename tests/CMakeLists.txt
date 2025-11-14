#tests/CmakeLists.txt


# Include the FetchContent module
include(FetchContent)

# Declare Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2  #  specific version for stability

)


FetchContent_MakeAvailable(googletest)






#  subdirectory for unit tests
add_subdirectory(unit)
#  subdirectory for integration tests
add_subdirectory(integration)
