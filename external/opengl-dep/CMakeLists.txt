# external/opengl-dep/CMakeLists.txt

find_package(OpenGL REQUIRED)


add_library(OpenGLDeps INTERFACE)


target_link_libraries(OpenGLDeps INTERFACE
    glad              
    OpenGL::GL        
)


target_include_directories(OpenGLDeps INTERFACE 

    ${CMAKE_CURRENT_SOURCE_DIR}/glad
)