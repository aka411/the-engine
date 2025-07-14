#core/CmakeLists.txt



add_library(TheEngine_Core INTERFACE)


target_include_directories(TheEngine_Core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>

)