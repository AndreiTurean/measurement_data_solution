# enable_testing()
# add_executable(${PROJECT_NAME}_test
# source/main.cxx
# source/EngineTests.cxx
# )
# add_test(NAME {PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)

# set_property(TEST {PROJECT_NAME}_test PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}")
# # packages are getted using the vcpkg package manager
# find_package(GTest CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

# gtest_add_tests(TARGET ${PROJECT_NAME}_test)

# target_link_libraries(${PROJECT_NAME}_test PRIVATE core::core) #link against the alias library

# install(TARGETS ${PROJECT_NAME}_test)