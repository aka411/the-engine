#root/CMakeLists.txt

cmake_minimum_required(VERSION 3.17)
project(TheEngine CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)



#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

if (ANDROID)
    list(FILTER ENGINE_SOURCES EXCLUDE REGEX "(windows|linux|desktop)_")
else()
    if (WIN32)
        list(FILTER ENGINE_SOURCES EXCLUDE REGEX "(linux|android)_")
    elseif (UNIX)
        list(FILTER ENGINE_SOURCES EXCLUDE REGEX "(windows|android)_")
    endif()
endif()







add_library(TheEngine STATIC

${ENGINE_SOURCES}

)









target_include_directories(TheEngine 

PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>"
)



add_subdirectory(external)

target_link_libraries(TheEngine PUBLIC

  common-lib

  VulkanDeps
  #OpenGLDeps
)







function(process_assets target)

    if (ANDROID)

        #Template generation handled it
    else()

        set(THE_ENGINE_BASE_ASSET_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets")
        if(EXISTS "${THE_ENGINE_BASE_ASSET_PATH}")

            configure_file(
            "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/config/engine_build_config.h.in"
            "${CMAKE_CURRENT_BINARY_DIR}/engine_build_config.h"           
            @ONLY
            )
        else()
            message(FATAL_ERROR  "Assets Folder does not exsist")
        endif()
    endif()
endfunction()








function (the_engine_build_app APP_TARGET)



    set(options  GENERATE_ANDROID_TEMPlATE)
    set(oneValueArgs   APP_NAME ANDROID_PACKAGE_NAME)
    set(multiValueArgs  SOURCES INCLUDES LIBRARIES)

    cmake_parse_arguments(ARG
        "${options}" "${oneValueArgs}" "${multiValueArgs}"
        ${ARGN}
    )

    if(NOT DEFINED ARG_SOURCES)
        message(FATAL_ERROR "Sources not given for ${APP_TARGET}")
    endif()

    # mm
    if(NOT DEFINED ARG_INCLUDES)
        message(WARNING "Includes not given for ${APP_TARGET}")
        set(ARG_INCLUDES "${CMAKE_CURRENT_LIST_DIR}")
    endif()

    if(NOT DEFINED ARG_LIBRARIES)
        message(WARNING "Includes not given for ${APP_TARGET}, falling back to ${CMAKE_CURRENT_LIST_DIR}")
        set(ARG_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}")
    endif()

    if(NOT DEFINED ARG_APP_NAME)
        set(ARG_APP_NAME "${APP_TARGET}")
    endif()


    if(ARG_GENERATE_ANDROID_TEMPlATE)

        include("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/android_template.cmake")

        set(ANDROID_WRAPPER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")

        if(NOT EXISTS "${ANDROID_WRAPPER_DIR}/app/build.gradle")

            message(STATUS "Generating Android Template")


            if(NOT DEFINED ARG_ANDROID_PACKAGE_NAME)
                message(FATAL_ERROR "ANDROID_PACKAGE_NAME not given")
                #set(ARG_ANDROID_PACKAGE_NAME "com.example.myapp")#Fix
            endif()


            the_engine_configure_android_template(
                ${APP_TARGET} 
                ANDROID_APP_DISPLAY_NAME "${ARG_APP_NAME}" 
                ANDROID_PACKAGE_NAME "${ARG_ANDROID_PACKAGE_NAME}")
        endif()
    endif()




    message(STATUS "Building app target: ${APP_TARGET}")

    if (ANDROID)

        add_library(${APP_TARGET} SHARED
           ${ARG_SOURCES}
        )

        
        target_include_directories(${APP_TARGET} PRIVATE
            ${ARG_INCLUDES}
        )



        find_package(game-activity REQUIRED CONFIG)


        set(CMAKE_SHARED_LINKER_FLAGS
            "${CMAKE_SHARED_LINKER_FLAGS} -u Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")

        target_compile_definitions(${APP_TARGET} PRIVATE PLATFORM_ANDROID)
        target_compile_definitions(TheEngine  PUBLIC

        PLATFORM_ANDROID
        VK_USE_PLATFORM_ANDROID_KHR

        )


        target_link_libraries(TheEngine PUBLIC
            game-activity::game-activity
            vulkan
            android
            log
            dl
        )

        target_link_libraries( ${APP_TARGET} PRIVATE
            TheEngine
            ${ARG_LIBRARIES}
        )



        set_target_properties(${APP_TARGET} PROPERTIES
            LINK_FLAGS "-u Java_com_google_androidgamesdk_GameActivity_initializeNativeCode"
        )

    else()
        

    

        add_executable(${APP_TARGET}
            ${ARG_SOURCES}
        )

        target_compile_definitions(${APP_TARGET}  PUBLIC PLATFORM_PC)
        target_compile_definitions(TheEngine  PUBLIC PLATFORM_PC)


        process_assets(${APP_TARGET})

        target_include_directories(TheEngine PRIVATE 
             "${CMAKE_CURRENT_BINARY_DIR}" # The asset file path config file will be in binary dir
        )

        target_include_directories( ${APP_TARGET} PRIVATE
            ${ARG_INCLUDES}
        )
        target_link_libraries( ${APP_TARGET} PRIVATE
            TheEngine
            ${ARG_LIBRARIES}
        )


    endif()
endfunction()




option(THE_ENGINE_BUILD_TESTS "Enable Tests" OFF)


if(THE_ENGINE_BUILD_TESTS)
    message(STATUS "Building Tests")
    target_compile_definitions(TheEngine PRIVATE PLATFORM_TEST)
    enable_testing()
    add_subdirectory(tests)
else()
    message(STATUS "Tests Not Built")
endif()




