cmake_minimum_required(VERSION 3.17)



function(the_engine_configure_android_template)

    set(ANDROID_TEMPLATE_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/android")
    set(ANDROID_CONFIGURED_TEMPLATE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")

    if(EXISTS "${ANDROID_CONFIGURED_TEMPLATE_DIR}")
        message(STATUS "${ANDROID_CONFIGURED_TEMPLATE_DIR} already exists aborting android template generation")
        return()
    endif()




    set(oneValueArgs  ANDROID_APP_DISPLAY_NAME ANDROID_PACKAGE_NAME)

    cmake_parse_arguments(ARG "" "${oneValueArgs}" "" ${ARGN})




    
    #APP_NAME_WITH_SPACE
    if(NOT DEFINED ARG_ANDROID_APP_DISPLAY_NAME OR NOT DEFINED ARG_ANDROID_PACKAGE_NAME)
        message(FATAL_ERROR "ANDROID_APP_DISPLAY_NAME or ANDROID_PACKAGE_NAME not specified, Please specify the names, Aborting....")
    endif()



    set(ANDROID_APP_DISPLAY_NAME_NORMAL "${ARG_ANDROID_APP_DISPLAY_NAME}")
    set(ANDROID_APP_DISPLAY_NAME_NO_SPACE "")
    set(ANDROID_APP_DISPLAY_NAME_NO_SPACE_LOWERCASE "")

    string(REPLACE " " "" ANDROID_APP_DISPLAY_NAME_NO_SPACE "${ARG_ANDROID_APP_DISPLAY_NAME}")
    string(TOLOWER "${ANDROID_APP_DISPLAY_NAME_NO_SPACE}" ANDROID_APP_DISPLAY_NAME_NO_SPACE_LOWERCASE)



    set(ANDROID_APP_NAME_PASCAL_CASE "${ANDROID_APP_DISPLAY_NAME_NO_SPACE}") #TODO : Implement later, "MyApp"
    set(ANDROID_APP_NAME_LOWER_CASE "${ANDROID_APP_DISPLAY_NAME_NO_SPACE_LOWERCASE}")#"myapp"

    set(ANDROID_PACKAGE_NAME "${ARG_ANDROID_PACKAGE_NAME}")
    set(REVERSE_DOMAIN_NAME "") #"com/example/myapp"
    string(REPLACE "." "/" REVERSE_DOMAIN_NAME "${ARG_ANDROID_PACKAGE_NAME}")



    file(GLOB_RECURSE TEMPLATE_FILES  RELATIVE "${ANDROID_TEMPLATE_DIR}/" "${ANDROID_TEMPLATE_DIR}/*")


    set(ANDROID_APP_SHARED_LIBRARY "${ANDROID_APP_DISPLAY_NAME_NORMAL}")

    set(CMAKE_AT_ESCAPE "@")# To fix issue with android:theme="@CMAKE_AT_ESCAPE@style/Theme.@ANDROID_APP_NAME@"> in AndroidManifest.xml
    set(THE_ENGINE_APP_ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets/")

    if(NOT EXISTS "${THE_ENGINE_APP_ASSETS_PATH}")
        file(MAKE_DIRECTORY "${THE_ENGINE_APP_ASSETS_PATH}")
        message(WARNING "Assets folder was missing, created one at ${THE_ENGINE_APP_ASSETS_PATH}")
    endif()


    set(APP_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")


    foreach(FILE_PATH IN LISTS TEMPLATE_FILES)

        set(IN_FILE "${ANDROID_TEMPLATE_DIR}/${FILE_PATH}")
    
        if(FILE_PATH MATCHES "domain/organisation/app-name")

            #There are 3 folder paths like this 
            string(REPLACE "domain/organisation/app-name" "${REVERSE_DOMAIN_NAME}" FILE_PATH "${FILE_PATH}")
        endif()


        set(OUT_FILE_PATH "${ANDROID_CONFIGURED_TEMPLATE_DIR}/${FILE_PATH}")

        # Skip text replacement for binary and asset files to prevent corruption
        if(FILE_PATH MATCHES "\\.(jar|png|jpg|webp|zip|jks|keystore|so)$")
            get_filename_component(OUT_DIR "${OUT_FILE_PATH}" DIRECTORY)
            file(COPY "${IN_FILE}" DESTINATION "${OUT_DIR}")
        else()
            configure_file("${IN_FILE}" "${OUT_FILE_PATH}" @ONLY)
        endif()

    endforeach()

    message(STATUS "Android project folders successfully created at: ${CMAKE_CURRENT_SOURCE_DIR}")
endfunction()