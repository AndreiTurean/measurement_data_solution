cmake_minimum_required(VERSION 3.22.0)

#######################################################
# target definition                                   #
#######################################################
add_library(${PROJECT_NAME}_raw MODULE
source/RawDataProcessor.cxx)

#   add the include directory to our target as PUBLIC
target_include_directories(${PROJECT_NAME}_raw PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>  # <prefix>/include/
)

#target_link_libraries(${PROJECT_NAME}_raw PRIVATE core)
target_link_libraries(${PROJECT_NAME}_raw PRIVATE defs)

#######################################################
# setting target compile feature/options              #
#######################################################
# https://cmake.org/cmake/help/latest/manual/cmake-compile-features.7.html
target_compile_features(${PROJECT_NAME}_raw PUBLIC cxx_std_17)


target_compile_definitions(${PROJECT_NAME}_raw PUBLIC DUMMY_DLL) # we want to build a shared library
target_compile_definitions(${PROJECT_NAME}_raw PRIVATE CREATE_DLL)



target_compile_options(${PROJECT_NAME}_raw PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4> # setting the W4 warning level for MSVC compilers
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror> # setting the Wall for other compilers
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fms-extensions>
)

#######################################################
# install step                                        #
#######################################################
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}_raw
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT RUNTIME
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT RUNTIME
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT DEVELOPMENT
)
#######################################################
# test step                                           #
#######################################################

if(BUILD_TESTING)
    # include(GoogleTest)
    # enable_testing()

    # add_executable(${PROJECT_NAME}_test
    # test/main.cxx)

    # # packages are getted using the vcpkg package manager
    # find_package(GTest REQUIRED)
    # target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

    # gtest_add_tests(TARGET ${PROJECT_NAME}_test)

    # target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}) #link against the alias library

    # install(TARGETS ${PROJECT_NAME}_test)
endif()