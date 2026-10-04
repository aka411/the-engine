#tests/CmakeLists.txt



include(FetchContent)


FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2 

)


FetchContent_MakeAvailable(googletest)





the_engine_get_the_engine_library(TheEngine)
target_compile_definitions(TheEngine PRIVATE PLATFORM_TEST)
enable_testing()


add_subdirectory(compile)

#  subdirectory for unit tests
#add_subdirectory(unit)
#  subdirectory for integration tests
#add_subdirectory(integration)

