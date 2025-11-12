#ecs/CMakeLists.txt

add_library(TheEngine_ECS STATIC

   "src/archetype_manager.cpp"
   "src/command_buffer.cpp" 
   "src/command_processor.cpp" 
   "src/bump_allocator.cpp" 
   "src/entity_manager.cpp"
   "src/component_registry.cpp"
   "src/ecs_manager.cpp"

)


target_include_directories(TheEngine_ECS PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>

)

