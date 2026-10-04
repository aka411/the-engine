cmake_minimum_required(VERSION 3.17)


project(AndroidCompileTest CXX C)


set(CMAKE_CXX_STANDARD 20)


file(GLOB_RECURSE APP_STUB_SOURCES
    "*.cpp"
    "*.h" 
)

the_engine_generate_android_template(
    APP_NAME "Android Compile Test Stub"
    ANDROID_PACKAGE_NAME "com.test.Stub"
)


the_engine_build_app(AndroidCompileTestStub

APP_NAME "Android Compile Test Stub"

COMPILE_ONLY_FOR_ANDROID ON

SOURCES
    ${APP_STUB_SOURCES}

INCLUDES
    "${CMAKE_CURRENT_LIST_DIR}"

)
