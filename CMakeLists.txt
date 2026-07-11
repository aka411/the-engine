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

${ENGINE_SOURCES}


)









target_include_directories(TheEngine PUBLIC

${CMAKE_CURRENT_SOURCE_DIR}/include

)



add_subdirectory(external)

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


enable_testing()

add_subdirectory(tests)






