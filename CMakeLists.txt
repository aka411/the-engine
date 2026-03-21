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

"external/glad/glad.c"

"src/rendering-system/gpu-resource-system/gpu_mesh_system.cpp"
"src/rendering-system/gpu-resource-system/gpu_material_manager.cpp"
"src/rendering-system/gpu-resource-system/gpu_animation_manager.cpp"
"src/rendering-system/gpu-resource-system/gpu_light_manager.cpp"
"src/rendering-system/gpu-resource-system/gpu_resource_system.cpp"

"src/rendering-system/low-level-gpu-systems/gpu_buffer_manager.cpp"
"src/rendering-system/low-level-gpu-systems/gpu_texture_manager.cpp"
"src/rendering-system/low-level-gpu-systems/gpu_sampler_manager.cpp" 
"src/rendering-system/low-level-gpu-systems/gpu_shader_manager.cpp" 
"src/rendering-system/low-level-gpu-systems/low_level_gpu_system.cpp" 




"src/rendering-system/low-level-gpu-systems/memory-management/gpu_buffer_transfer_manager.cpp"

"src/rendering-system/low-level-gpu-systems/memory-management/gpu-allocators/i_gpu_buffer_suballocator.cpp"
"src/rendering-system/low-level-gpu-systems/memory-management/gpu-allocators/gpu_buffer_tlsf_suballocator.cpp"
"src/rendering-system/low-level-gpu-systems/memory-management/gpu-allocators/gpu_buffer_per_frame_suballocator.cpp"
"src/rendering-system/low-level-gpu-systems/memory-management/gpu-allocators/gpu_buffer_fixed_size_suballocator.cpp"
"src/rendering-system/low-level-gpu-systems/memory-management/gpu-allocators/gpu_buffer_circular_suballocator.cpp"
"src/rendering-system/low-level-gpu-systems/memory-management/gpu-allocators/gpu_buffer_bump_suballocator.cpp"





"src/rendering-system/renderer/rendering_context.cpp"

"src/rendering-system/renderer/rendering_system.cpp"


"src/rendering-system/renderer/render-pass/i_render_pass.cpp"
"src/rendering-system/renderer/render-pass/gbuffer_render_pass.cpp"

"src/animation-system/animation_system.cpp"


"src/asset-system/material_system.cpp"
"src/asset-system/animation_resource_system.cpp"
"src/asset-system/light_system.cpp"
"src/asset-system/mesh_system.cpp"
"src/asset-system/texture_system.cpp"
"src/asset-system/asset_system.cpp"



"src/engine/engine.cpp" 



"src/rendering-system/api-backend/opengl/opengl_texture_helper.cpp"
"src/rendering-system/api-backend/opengl/opengl_utils.cpp"
"src/rendering-system/api-backend/opengl/opengl_buffer_helper.cpp"


"src/platform/platform.cpp"
"src/platform/file_system.cpp"
"src/platform/input_system.cpp"
"src/platform/window_system.cpp"



"src/rendering-system/api-backend/opengl/opengl_shader_compiler.cpp"  

"src/rendering-system/renderer/render-pass/forward_render_pass.cpp" 
"src/rendering-system/low-level-gpu-systems/gpu_pipeline_manager.cpp"
"include/rendering-system/low-level-gpu-systems/data-structures/gpu_pipeline_state_data_types.h"



"src/rendering-system/api-backend/opengl/opengl_pipeline_helper.cpp"



"src/platform/path.cpp"
"src/platform/file.cpp"


)


 set_target_properties(TheEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")



target_include_directories(TheEngine PUBLIC

${CMAKE_CURRENT_SOURCE_DIR}/include

${CMAKE_CURRENT_SOURCE_DIR}/external/

${CMAKE_CURRENT_SOURCE_DIR}/external/ecs-engine/include



)

#target_include_directories(opengl-rendering-engine PUBLIC ${glm_INCLUDE_DIR})

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
