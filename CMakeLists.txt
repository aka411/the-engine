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






add_subdirectory(external/ecs-engine)



add_library(TheEngine STATIC



 "external/glad/glad.c"


 "src/common_header_imp.cpp"
 "src/camera.cpp"

 "src/model-loading/gltf_intermediate_mapper.cpp"
 "src/model-loading/data_convertor.cpp"
 "src/model-loading/gltf_animation_extractor.cpp"
 "src/model-loading/vertex_attributes_repacker.cpp"
 "src/model-loading/gltf_attribute_extractor.cpp"

 "src/engine/engine_core.cpp"
 "src/engine/engine_loader.cpp"

 "src/low-level/vertex_format_helper.cpp"
 "src/low-level/vertex_format_manager.cpp"

 "src/memory-management/gpu_buffer_manager.cpp"
 "src/memory-management/gpu_buffer_sub_bump_allocator.cpp"

 "src/low-level/gpu_texture_manager.cpp"
 "src/low-level/gpu_material_system.cpp"

 "src/low-level/shader_code_generator.cpp"
 "src/low-level/shader_manager.cpp"

 "src/low-level/world_vertex_buffer_management_system.cpp"







 "src/rendering-system/render_system.cpp"
 "src/rendering-system/world_renderer.cpp"




 "src/ecs_hierarchy_traverser.cpp"
 "src/transformation_system.cpp"

 # UI SOURCE FILES

 "src/ui/core/ui_system.cpp"


 "src/ui/builder/ui_element.cpp"
 "src/ui/builder/ui_text_label_builder.cpp"
 "src/ui/builder/ui_graph_builder.cpp"
 "src/ui/builder/ui_builder.cpp"
 "src/ui/builder/ui_base_builder.cpp"
 "src/ui/builder/ui_panel_builder.cpp"


 "src/ui/systems/ui_vertex_buffer_management_system.cpp"
 "src/ui/systems/ui_mesh_system.cpp"
 "src/ui/systems/ui_transformation_system.cpp"
 "src/ui/systems/performance_monitor_system.cpp"
 "src/ui/systems/ui_layout_system.cpp"

 "src/ui/text/ui_text_system.cpp"
 "src/ui/text/json_font_parser.cpp"
 "src/ui/text/ui_text_generation_system.cpp"
  "src/ui/text/ui_font_manager.cpp"

 "src/ui/core/ui_shader_manager.cpp"
 "src/ui/core/ui_core_system.cpp"

 "src/ui/ui_renderer.cpp"

 "src/ui/utils/interleaver.cpp"
 "src/ui/utils/geometry_generator.cpp"

 "src/ui/builder/ui_window_builder.cpp"



 "src/animation/animation_system.cpp"

 "src/animation/keyframe_animation_manager.cpp"
 "src/animation/skeletal_animation_manager.cpp"

 "src/animation/keyframe_animation_interpolator.cpp"
 "src/animation/skeletal_animation_solver.cpp"


 "src/memory-management/gpu_buffer_per_frame_allocator.cpp"


 "src/rendering-system/object_data_buffer_manager.cpp"
 "src/rendering-system/render_command_buffer_manager.cpp"


"src/platform.cpp" 
"src/rendering-engine/opengl-backend/opengl_utils.cpp" 


 "src/utils/file-decoders/image_decoder.cpp"

 "src/memory-management/gpu_buffer_circular_allocator.cpp"

 "src/rendering-engine/opengl-backend/opengl_texture_helper.cpp"


)


 set_target_properties(TheEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")



target_include_directories(TheEngine PUBLIC

${CMAKE_CURRENT_SOURCE_DIR}/include

${CMAKE_CURRENT_SOURCE_DIR}/external/

${CMAKE_CURRENT_SOURCE_DIR}/external/ecs-engine/include

#${CMAKE_CURRENT_SOURCE_DIR}/ui/include

)

#target_include_directories(opengl-rendering-engine PUBLIC ${glm_INCLUDE_DIR})
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_LIBC ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.2.20
)

FetchContent_MakeAvailable(SDL3)


FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	1.0.1 #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)


target_link_libraries(TheEngine PUBLIC
  SDL3::SDL3-static
  #SDL3::SDL3
  glm::glm
  TheEngine_ECS
)


add_subdirectory(demo)


  #add_custom_command(TARGET TheEngine POST_BUILD
  #  COMMAND ${CMAKE_COMMAND} -E copy_directory
  #  $<TARGET_FILE_DIR:SDL3::SDL3>
  # $<TARGET_FILE_DIR:TheEngine>
  #  COMMENT "Copying SDL3 DLL to the executable directory"
  #)
