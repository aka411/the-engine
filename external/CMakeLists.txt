# external/CMakeLists.txt
include(FetchContent)





FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.2.20
)

set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_LIBC ON CACHE BOOL "" FORCE)



set(SDL_AUDIO OFF CACHE BOOL "Disable audio" FORCE)
set(SDL_CAMERA OFF CACHE BOOL "Disable camera" FORCE)
set(SDL_HAPTIC OFF CACHE BOOL "Disable haptic" FORCE)
set(SDL_HIDAPI OFF CACHE BOOL "Disable hidapi" FORCE)
set(SDL_JOYSTICK OFF CACHE BOOL "Disable joystick" FORCE)
set(SDL_POWER OFF CACHE BOOL "Disable power" FORCE)
set(SDL_SENSOR OFF CACHE BOOL "Disable sensor" FORCE)
set(SDL_DIALOG OFF CACHE BOOL "Disable dialog" FORCE)


set(SDL_TEST_LIBRARY OFF CACHE BOOL "Do not build SDL3_test library" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "Do not build examples" FORCE)
set(SDL_TESTS OFF CACHE BOOL "Do not build tests" FORCE)

set(SDL_RENDER_D3D OFF CACHE BOOL "" FORCE)
set(SDL_RENDER_D3D11 OFF CACHE BOOL "" FORCE)
set(SDL_RENDER_D3D12 OFF CACHE BOOL "" FORCE)
set(SDL_RENDER_GPU OFF CACHE BOOL "" FORCE)
set(SDL_DIRECTX OFF CACHE BOOL "" FORCE)
set(SDL_OPENGL OFF CACHE BOOL "" FORCE)
set(SDL_OPENGLES OFF CACHE BOOL "" FORCE)

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