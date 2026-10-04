cmake_minimum_required(VERSION 3.14)


project(AndroidCompileTest CXX C)


set(CMAKE_CXX_STANDARD 20)


file(GLOB_RECURSE APP_STUB_SOURCES
    "*.cpp"
    "*.h" 
)




the_engine_build_app(AndroidCompileTestStub

APP_NAME "Android Compile Test Stub"
ANDROID_PACKAGE_NAME "com.test.Stub"

GENERATE_ANDROID_TEMPlATE ON

SOURCES
    ${APP_STUB_SOURCES}

INCLUDES
    "${CMAKE_CURRENT_LIST_DIR}"

)
