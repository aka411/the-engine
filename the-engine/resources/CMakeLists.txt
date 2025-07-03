#the-engine/resources/CMakeLists.txt




add_library(TheEngine_Resource SHARED
    src/cpu_resource_manager.cpp
   
)


target_include_directories(TheEngine_Resource PUBLIC

    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
