#root/CMakeLists.txt

cmake_minimum_required(VERSION 3.17)
project(TheEngine CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)





set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


file(GLOB_RECURSE ENGINE_SOURCES
    "src/*.cpp" 
    "src/*.h" 
    "src/*.hpp"
    "include/*.h"
)


file(GLOB_RECURSE EXCLUDE_SOURCES 
"src/rendering-system/api-backend/opengl/*"
"include/rendering-system/api-backend/opengl/*"
)

list(REMOVE_ITEM ENGINE_SOURCES ${EXCLUDE_SOURCES})



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

 "src/light-system/light_system.cpp"
 

 

 "src/ui/utils/ui_geometry_generator.cpp"


)









target_include_directories(TheEngine PUBLIC

${CMAKE_CURRENT_SOURCE_DIR}/include

)



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

  common-lib

  VulkanDeps
  #OpenGLDeps

)


function(copy_runtime_deps target)
    if(WIN32)
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_RUNTIME_DLLS:${target}>"
            "$<TARGET_FILE_DIR:${target}>"
            COMMAND_EXPAND_LISTS
        )
    endif()
endfunction()





option(BUILD_TESTS "Enable Tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()




