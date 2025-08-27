#graphics/CMakeLists.txt

add_library(TheEngine_Graphics SHARED

 
)


target_include_directories(TheEngine_Graphics PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>

)




