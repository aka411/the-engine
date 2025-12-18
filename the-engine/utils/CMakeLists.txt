


add_library(TheEngine_Utils INTERFACE)

target_include_directories(TheEngine_Utils INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)