


include(FetchContent)


set(CMAKE_POSITION_INDEPENDENT_CODE ON)

FetchContent_Declare(
  volk
  GIT_REPOSITORY https://github.com/zeux/volk.git
  GIT_TAG        vulkan-sdk-1.4.321.0
)

#guide volk to where vulkan headers are
set(VULKAN_HEADERS_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vulkan-headers" CACHE PATH "" FORCE)
FetchContent_MakeAvailable(volk)

if (ANDROID)
    if (TARGET volk)
        target_compile_definitions(volk PUBLIC VK_USE_PLATFORM_ANDROID_KHR)
    endif()
endif()




FetchContent_Declare(
  VulkanBootstrap
  GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap.git
  GIT_TAG        v1.4.321


)
set(VK_BOOTSTRAP_VULKAN_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vulkan-headers/include" CACHE PATH "" FORCE)
FetchContent_MakeAvailable(VulkanBootstrap)
set_target_properties(vk-bootstrap PROPERTIES POSITION_INDEPENDENT_CODE ON)

FetchContent_Declare(
  VMA
  GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
  GIT_TAG        v3.3.0
)


FetchContent_MakeAvailable(VMA)




FetchContent_Declare(
  spirv-reflect
  GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Reflect.git
  GIT_TAG        vulkan-sdk-1.3.290.0

)
set(SPIRV_REFLECT_EXECUTABLE OFF CACHE BOOL "" FORCE)
set(SPIRV_REFLECT_STATIC_LIB ON CACHE BOOL "" FORCE)
set(SPIRV_REFLECT_BUILD_TESTS OFF CACHE BOOL "" FORCE)

#OPTION(SPIRV_REFLECT_ENABLE_ASSERTS "Enable asserts for debugging" OFF)
#OPTION(SPIRV_REFLECT_ENABLE_ASAN    "Use address sanitization" OFF)
#OPTION(SPIRV_REFLECT_INSTALL        "Whether to install" ON)

FetchContent_MakeAvailable(spirv-reflect)





FetchContent_Declare(
  shaderc-prebuilt-lib
  GIT_REPOSITORY https://github.com/aka411/the-engine-prebuilt-deps.git
  GIT_TAG        ec3ce4278a6173065e7602222ba6de0cc42e0cd5

)
FetchContent_MakeAvailable(shaderc-prebuilt-lib)



add_library(VulkanDeps INTERFACE)


target_link_libraries(VulkanDeps INTERFACE
  volk
  vk-bootstrap
  VulkanMemoryAllocator

  shaderc::shaderc_shared

  spirv-reflect-static

  )

