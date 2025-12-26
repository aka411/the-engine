#demo/CMakeLists.txt

cmake_minimum_required(VERSION 3.17) # FetchContent_MakeAvailable requires 3.14+
project(Demo CXX C)


add_executable(DemoApp

"main.cpp"
"demo_app.cpp"

)

target_link_libraries(DemoApp PUBLIC TheEngine)


