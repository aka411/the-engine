#unit/CMakeLists.txt



include(GoogleTest)


add_executable(SanityCheckTests sanity_check.cpp)



target_link_libraries(SanityCheckTests PRIVATE
    GTest::gtest_main
)








#add_executable(GPUResourceSystemTests
   #graphics/gpu_resource_manager_test.cpp
  # graphics/gpu_resource_system_test.cpp
#)

#target_link_libraries(
   # GPUResourceSystemTests PRIVATE
   # GTest::gtest_main
  #  GTest::gmock_main


    #TheEngine_Graphics
    #TheEngine_Resource
    #TheEngine_Core
#)




#[[

add_executable(ecs-system-test

  ecs/archetype_manager_test.cpp
  ecs/entity_manager_test.cpp
  
)

target_link_libraries(
    ecs-system-test PRIVATE
    GTest::gtest_main
    GTest::gmock_main
    TheEngine_ECS
    TheEngine_Utils
    #TheEngine_Graphics
    #TheEngine_Resource
    #TheEngine_Core
)

]]






#gtest_discover_tests(ecs-system-test)

gtest_discover_tests(SanityCheckTests)

#gtest_discover_tests(CPUResourceManagerTests )

#gtest_discover_tests(GPUResourceSystemTests)
