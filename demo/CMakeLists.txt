#demo/CMakeLists.txt

cmake_minimum_required(VERSION 3.17) # FetchContent_MakeAvailable requires 3.14+
project(Demo CXX C)


add_executable(DemoApp

"src/main.cpp"
"src/demo_app.cpp"

)

target_link_libraries(DemoApp PUBLIC TheEngine)


target_include_directories(DemoApp PRIVATE

${CMAKE_CURRENT_SOURCE_DIR}/include

)


add_custom_command(TARGET DemoApp POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/demo/asset $<TARGET_FILE_DIR:DemoApp>/asset

)