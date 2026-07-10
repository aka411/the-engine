


include(FetchContent)




FetchContent_Declare(
  volk
  GIT_REPOSITORY https://github.com/zeux/volk.git
  GIT_TAG        vulkan-sdk-1.4.321.0
)

#guide volk to where vulkan headers are
set(VULKAN_HEADERS_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vulkan-headers" CACHE PATH "" FORCE)
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




# SHADERC
set(SHADERC_LIB_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libshaderc/prebuilt-libs/")
if(WIN32)
    if(MSVC)
        #win64_msvc/shaderc_shared.dll"
        set(SHADERC_PLATFORM "win64_msvc")
    elseif(MINGW)
        message(FATAL_ERROR "MINGW compiler not yet supported for prebuilt shaderc")
        set(SHADERC_PLATFORM "win64_mingw")
    else()
        message(FATAL_ERROR "Unsupported compiler for prebuilt shaderc")
    endif()
elseif(UNIX)
 message(FATAL_ERROR "Unix not yet supported.")
else()
    message(FATAL_ERROR "Platform not supported.")
endif()    


add_library(shaderc_shared SHARED IMPORTED)

set_target_properties(shaderc_shared PROPERTIES
    IMPORTED_LOCATION "${SHADERC_LIB_ROOT_PATH}/${SHADERC_PLATFORM}/shaderc_shared.dll"
    IMPORTED_IMPLIB "${SHADERC_LIB_ROOT_PATH}/${SHADERC_PLATFORM}/shaderc_shared.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libshaderc/include"
)










add_library(VulkanDeps INTERFACE)


target_link_libraries(VulkanDeps INTERFACE
  volk
  vk-bootstrap
  VulkanMemoryAllocator

  shaderc_shared

  spirv-reflect-static

  )

