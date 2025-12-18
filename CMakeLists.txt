#root/CMakeLists.txt

cmake_minimum_required(VERSION 3.15) # FetchContent_MakeAvailable requires 3.14+
project(TheEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)




enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


add_subdirectory(the-engine)

# Add the tests directory.
# This subdirectory will handle fetching Google Test and defining test executables.
#add_subdirectory(tests) #IMPORTANT UNCOMMENT TO INCLUDE TEST FOLDER FOR TESTING
