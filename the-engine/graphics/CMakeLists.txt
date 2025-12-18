#graphics/CMakeLists.txt

#add_library(TheEngine_Graphics SHARED


#)


#target_include_directories(TheEngine_Graphics PUBLIC
#    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>

#)






#cmake_minimum_required(VERSION 3.17)
project(vulkan-renderer-prototype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)


#[[
FetchContent_Declare(
shaderc
GIT_REPOSITORY https://github.com/google/shaderc.git
GIT_TAG v2025.4

)



set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Set build type" FORCE)

# Explicitly set the variables to disable tests, examples, and executables
set(SHADERC_SKIP_TESTS ON CACHE BOOL "Skip building Shaderc tests." FORCE)
#set(SHADERC_ENABLE_TESTS OFF CACHE BOOL "Disable building Shaderc tests." FORCE)
set(SHADERC_SKIP_EXAMPLES ON CACHE BOOL "Skip building Shaderc examples." FORCE) # Optional, but good practice
#set(SHADERC_ENABLE_EXAMPLES OFF CACHE BOOL "Disable building Shaderc examples." FORCE) # Optional, but good practice
#set(SHADERC_SKIP_EXECUTABLES ON CACHE BOOL "Skip building Shaderc executables." FORCE)
#set(SHADERC_ENABLE_EXECUTABLES OFF CACHE BOOL "Disable building Shaderc executables." FORCE)

FetchContent_MakeAvailable(shaderc)
]]



set(VULKAN_SDK_Path "C:/VulkanSDK/1.4.321.1")

set(SPIRV_Tools_DIR "${VULKAN_SDK_Path}/Lib/cmake/SPIRV-Tools/cmake")
set(SHADERC_INCLUDE_DIR "${VULKAN_SDK_Path}/Include/shaderc")
set(SHADERC_LIBRARY "${VULKAN_SDK_Path}/Lib/shaderc_combinedd.lib")



add_library(shaderc STATIC IMPORTED)
set_target_properties(shaderc PROPERTIES
    IMPORTED_LOCATION "${SHADERC_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${SHADERC_INCLUDE_DIR}"
)




add_compile_definitions(VK_NO_PROTOTYPES)


FetchContent_Declare(
  volk
  GIT_REPOSITORY https://github.com/zeux/volk.git
  GIT_TAG        vulkan-sdk-1.4.321.0
)

FetchContent_MakeAvailable(volk)





FetchContent_Declare(
  VulkanBootstrap
  GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap.git
  GIT_TAG        v1.4.321


)

FetchContent_MakeAvailable(VulkanBootstrap)


FetchContent_Declare(
  VMA
  GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
  GIT_TAG        v3.3.0
)


FetchContent_MakeAvailable(VMA)


FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.2.20
)

FetchContent_MakeAvailable(SDL3)


FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	1.0.1 #refs/tags/1.0.1
)


FetchContent_MakeAvailable(glm)






add_executable(vulkan_renderer_prototype
src/main.cpp
"src/vulkan_device.cpp"
 "src/queue_manager.cpp"
 "src/vulkan_renderer.cpp"
 "src/shader_manager.cpp"
 "src/buffer_manager.cpp"
 "src/swap_chain_manager.cpp"
 "src/image_manager.cpp"
 "src/pipeline_manager.cpp"
 "src/descriptor_set_manager.cpp"

)

target_include_directories(vulkan_renderer_prototype PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include

  )





target_link_libraries(vulkan_renderer_prototype PRIVATE
  volk
  vk-bootstrap
  VulkanMemoryAllocator

  SDL3::SDL3
  glm::glm

  shaderc
  )

  add_custom_command(TARGET vulkan_renderer_prototype POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:SDL3::SDL3>
    $<TARGET_FILE_DIR:vulkan_renderer_prototype>
    COMMENT "Copying SDL3 DLL to the executable directory"
  )
