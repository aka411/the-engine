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







add_library(TheEngine STATIC


)


set_target_properties(TheEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")



target_include_directories(TheEngine PUBLIC

${CMAKE_CURRENT_SOURCE_DIR}/include

${CMAKE_CURRENT_SOURCE_DIR}/external/


)



# SDL is used for input handling and window management, in vulkan we also use it for surface creation
# I can switch to glfw also if i want cause the way the engine is designed is very modular 
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.2.20
)

set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_LIBC ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3)


FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	1.0.1 
)

FetchContent_MakeAvailable(glm)


target_link_libraries(TheEngine PUBLIC
  SDL3::SDL3-static
  #SDL3::SDL3
  glm::glm
)





  #add_custom_command(TARGET TheEngine POST_BUILD
  #  COMMAND ${CMAKE_COMMAND} -E copy_directory
  #  $<TARGET_FILE_DIR:SDL3::SDL3>
  # $<TARGET_FILE_DIR:TheEngine>
  #  COMMENT "Copying SDL3 DLL to the executable directory"
  #)
