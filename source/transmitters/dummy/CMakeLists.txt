cmake_minimum_required(VERSION 3.22.0)

#######################################################
# project definition                                  #
#######################################################

add_library(${PROJECT_NAME}_dummy MODULE
source/DummyObject.cxx)

#   add the include directory to our target as PUBLIC
target_include_directories(${PROJECT_NAME}_dummy PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>  # <prefix>/include/
)

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}_dummy PRIVATE defs)
find_package(imgui)
target_link_libraries(${PROJECT_NAME}_dummy PRIVATE imgui::imgui)
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}_dummy PRIVATE SDL2::SDL2)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_libraries(${PROJECT_NAME}_dummy PRIVATE SDL2::SDL2main)
else()
    target_link_libraries(${PROJECT_NAME}_dummy PRIVATE SDL2::SDL2-static)
endif()

find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME}_dummy PRIVATE GLEW::GLEW)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}_dummy PRIVATE glfw)

#######################################################
# setting target compile feature/options              #
#######################################################
# https://cmake.org/cmake/help/latest/manual/cmake-compile-features.7.html
target_compile_features(${PROJECT_NAME}_dummy PUBLIC cxx_std_17)


target_compile_definitions(${PROJECT_NAME}_dummy PUBLIC DUMMY_DLL) # we want to build a shared library
target_compile_definitions(${PROJECT_NAME}_dummy PRIVATE CREATE_DLL)



target_compile_options(${PROJECT_NAME}_dummy PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4> # setting the W4 warning level for MSVC compilers
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror -pthread> # setting the Wall for other compilers
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fms-extensions>
)

#######################################################
# install step                                        #
#######################################################
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}_dummy
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT RUNTIME
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT RUNTIME
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT DEVELOPMENT
)

install(TARGETS ${PROJECT_NAME}_dummy RUNTIME_DEPENDENCIES DESTINATION ${CMAKE_INSTALL_BINDIR})