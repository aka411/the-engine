# external/CMakeLists.txt
include(FetchContent)





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
set(SDL_TEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3)


FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	1.0.1 
)

FetchContent_MakeAvailable(glm)



add_library(common-lib STATIC

"common-dep/miniaudio/miniaudio.c"

)


target_include_directories(common-lib INTERFACE

${CMAKE_CURRENT_SOURCE_DIR}/common-dep

)


#add_subdirectory(common-dep)



add_subdirectory(vulkan-dep)
#add_subdirectory(opengl-dep)