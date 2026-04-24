#root/CMakeLists.txt

cmake_minimum_required(VERSION 3.17) # FetchContent_MakeAvailable requires 3.14+
project(TheEngine CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)


#enable_testing()


# Add the tests directory.
# This subdirectory will handle fetching Google Test and defining test executables.
#add_subdirectory(tests) #IMPORTANT UNCOMMENT TO INCLUDE TEST FOLDER FOR TESTING


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


file(GLOB_RECURSE ENGINE_SOURCES 
    "src/*.cpp" 
    "src/*.h" 
    "src/*.hpp"
    "include/*.h"
)




add_library(TheEngine STATIC

${ENGINE_SOURCES}



)


set_target_properties(TheEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")



target_include_directories(TheEngine PUBLIC

${CMAKE_CURRENT_SOURCE_DIR}/include

)



add_subdirectory(external)

target_link_libraries(TheEngine PUBLIC

  SDL3::SDL3-static
  #SDL3::SDL3
  glm::glm

  common-lib


)



enable_testing()



# This subdirectory will handle fetching Google Test and defining test executables.
add_subdirectory(tests) #IMPORTANT UNCOMMENT TO INCLUDE TEST FOLDER FOR TESTING






  #add_custom_command(TARGET TheEngine POST_BUILD
  #  COMMAND ${CMAKE_COMMAND} -E copy_directory
  #  $<TARGET_FILE_DIR:SDL3::SDL3>
  # $<TARGET_FILE_DIR:TheEngine>
  #  COMMENT "Copying SDL3 DLL to the executable directory"
  #)
