cmake_minimum_required(VERSION 3.22.0)

#######################################################
# setting RPATH                                       #
#######################################################

set(CMAKE_INSTALL_RPATH ${ORIGIN})

#######################################################
# output artifacts redirect                           #
#######################################################
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#######################################################
# misc                                                #
#######################################################

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_DEBUG_POSTFIX "_debug")

# Set the build type if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

enable_testing()

#######################################################
# project definition                                  #
#######################################################
#   setting the project name, version and language
project(mds     VERSION 0.1.0
	DESCRIPTION "Project description"
	HOMEPAGE_URL "https://github.com/AndreiTurean/measurement_data_solution"
	LANGUAGES CXX)

add_subdirectory(source/definitions)
add_subdirectory(source/utilis)
add_subdirectory(source/replay)
add_subdirectory(source/processors)
add_subdirectory(source/transmitters)
add_subdirectory(source/recording)
add_subdirectory(source/core)
add_subdirectory(source/application)
add_subdirectory(source/test)

#######################################################
# package versioning                                  #
#######################################################

include(CMakePackageConfigHelpers)
configure_package_config_file(${PROJECT_NAME}.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.cmake
  INSTALL_DESTINATION ${LIB_INSTALL_DIR}/${PROJECT_NAME}/cmake)

write_basic_package_version_file(
  ${PROJECT_NAME}ConfigVersion.cmake
  VERSION 0.1.0
  COMPATIBILITY SameMajorVersion
)

#######################################################
# cpack                                               #
#######################################################
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_VERSION_MINOR})

include(CPack)