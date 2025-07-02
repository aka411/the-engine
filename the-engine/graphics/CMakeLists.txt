#graphics

add_library(TheEngine::Graphics STATIC
    #src/render_system.cpp
   
 
)


target_include_directories(TheEngine::Graphics PUBLIC

    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
)


target_include_directories(TheEngine::Graphics PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)


#target_link_libraries(TheEngine::Graphics PRIVATE TheEngine::Core)