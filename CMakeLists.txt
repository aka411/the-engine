#root/CMakeLists.txt

cmake_minimum_required(VERSION 3.17)
project(TheEngine CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)



#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)



function(the_engine_get_the_engine_library TheEngineOut)


    if(TARGET TheEngine)
        set(${TheEngineOut} TheEngine PARENT_SCOPE)
        return()
    endif()

    file(GLOB_RECURSE ENGINE_SOURCES
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/*.h" 
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/*.h"
    )


    file(GLOB_RECURSE EXCLUDE_SOURCES 
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/rendering-system/api-backend/opengl/*"
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/rendering-system/api-backend/opengl/*"

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
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include>"
    PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src>"
    )



    add_subdirectory("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/external" "${CMAKE_CURRENT_BINARY_DIR}/external")

    target_link_libraries(TheEngine PUBLIC

      common-lib

      VulkanDeps
      #OpenGLDeps
    )

    set(${TheEngineOut} ${TheEngine} PARENT_SCOPE)

endfunction()







function(process_assets)

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





function (the_engine_generate_android_template)

    if(ANDROID)
        return()
    endif()

    set(ANDROID_WRAPPER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")

    if(EXISTS "${ANDROID_WRAPPER_DIR}" AND EXISTS "${ANDROID_WRAPPER_DIR}/app/build.gradle")
        message(STATUS "Skipping Android Template Generation to prevent Overwrite")
        return()
    endif()




    include("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/android_template.cmake")

    set(oneValueArgs   APP_NAME ANDROID_PACKAGE_NAME)

    cmake_parse_arguments(ARG
        "" "${oneValueArgs}" ""
        ${ARGN}
    )



    if(NOT DEFINED ARG_APP_NAME)
         message(FATAL_ERROR "APP_NAME not specifed, cannot generate android template")
    endif()
    if(NOT DEFINED ARG_ANDROID_PACKAGE_NAME)
         message(FATAL_ERROR "ANDROID_PACKAGE_NAME not specifed, cannot generate android template")
    endif()

    message(STATUS "Generating Android Template")

    the_engine_configure_android_template(
        ANDROID_APP_DISPLAY_NAME "${ARG_APP_NAME}" 
        ANDROID_PACKAGE_NAME "${ARG_ANDROID_PACKAGE_NAME}")

endfunction()





function (the_engine_build_app)


    set(options COMPILE_ONLY_FOR_ANDROID)
    set(oneValueArgs   APP_NAME)
    set(multiValueArgs  SOURCES INCLUDES LIBRARIES)

    cmake_parse_arguments(ARG
        "${options}" "${oneValueArgs}" "${multiValueArgs}"
        ${ARGN}
    )

    if(NOT DEFINED ARG_SOURCES)
        message(FATAL_ERROR "Sources not given for compilation")
    endif()


    if(NOT DEFINED ARG_INCLUDES)
        message(WARNING "Includes not given for compilation, using current directory")
        set(ARG_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()


    if(NOT DEFINED ARG_APP_NAME)
        message(FATAL_ERROR "APP_NAME not given, aborting")
        return()
    endif()



    if (ANDROID)

        the_engine_get_the_engine_library(TheEngine)
        add_library(APP_TARGET SHARED
           ${ARG_SOURCES}
        )

        set_target_properties(APP_TARGET PROPERTIES
            OUTPUT_NAME "${ARG_APP_NAME}"
        )

        target_include_directories(APP_TARGET PRIVATE
            ${ARG_INCLUDES}
        )

        

        find_package(game-activity REQUIRED CONFIG)


        set(CMAKE_SHARED_LINKER_FLAGS
            "${CMAKE_SHARED_LINKER_FLAGS} -u Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")

        target_compile_definitions(APP_TARGET PRIVATE PLATFORM_ANDROID)
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

        target_link_libraries(APP_TARGET PRIVATE
            TheEngine
            ${ARG_LIBRARIES}
        )



        set_target_properties(APP_TARGET PROPERTIES
            LINK_FLAGS "-u Java_com_google_androidgamesdk_GameActivity_initializeNativeCode"
        )

    elseif(NOT ARG_COMPILE_ONLY_FOR_ANDROID)
        
        the_engine_get_the_engine_library(TheEngine)
        add_executable(APP_TARGET
            ${ARG_SOURCES}
        )
    
        set_target_properties(APP_TARGET PROPERTIES
            OUTPUT_NAME "${ARG_APP_NAME}"
        )


        target_compile_definitions(APP_TARGET  PUBLIC PLATFORM_PC)
        target_compile_definitions(TheEngine  PUBLIC PLATFORM_PC)


        process_assets()

        target_include_directories(TheEngine PRIVATE 
             "${CMAKE_CURRENT_BINARY_DIR}" # The asset file path config file will be in binary dir
        )

        target_include_directories(APP_TARGET PRIVATE
            ${ARG_INCLUDES}
        )

        target_link_libraries(APP_TARGET PRIVATE
            TheEngine
        )

        if(ARG_LIBRARIES)
            target_link_libraries(APP_TARGET PRIVATE
                ${ARG_LIBRARIES}
            )
        endif()
    else()
        message(STATUS "Settings are set such that it will only compile for android")
    endif()
endfunction()




option(THE_ENGINE_BUILD_TESTS "Enable Tests" OFF)


if(THE_ENGINE_BUILD_TESTS)
    message(STATUS "Building Tests")
    add_subdirectory(tests)
else()
    message(STATUS "Tests Not Built")
endif()




