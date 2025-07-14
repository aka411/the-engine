#graphics/CMakeLists.txt

add_library(TheEngine_Graphics SHARED
    #src/render_system.cpp
   src/gpu_resource_system/gpu_resource_manager.cpp
   src/gpu_resource_system/gpu_resource_system.cpp
 
)


target_include_directories(TheEngine_Graphics PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/gpu_resource_system>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/gpu_resource_system/converter>
)


#target_include_directories(TheEngine::Graphics PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/src
#)


#target_link_libraries(TheEngine::Graphics PRIVATE TheEngine::Core)