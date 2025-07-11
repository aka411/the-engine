#graphics

add_library(TheEngine_Graphics SHARED
    #src/render_system.cpp
   src/gpu_resource_manager.cpp
 
)


target_include_directories(TheEngine_Graphics PRIVATE

    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)


#target_include_directories(TheEngine::Graphics PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/src
#)


#target_link_libraries(TheEngine::Graphics PRIVATE TheEngine::Core)