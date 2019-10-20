cmake_minimum_required(VERSION 3.15)
project(
  "Simple-XML"
  VERSION 0.5.0
  DESCRIPTION
    "Abridged XML grammar to parse simple XML files like from MS Excel.")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
# This is an object library project
#
add_library(${PROJECT_NAME} OBJECT)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
file(
  GLOB SRC_FILES
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
file(
  GLOB PUB_FILES
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/public/*)
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES} PUBLIC ${PUB_FILES})
# Debug dump of globs:
foreach(src_file ${SRC_FILES})
  message(STATUS "${PROJECT_NAME} src file: ${src_file}")
endforeach(src_file)
foreach(pub_file ${PUB_FILES})
  message(STATUS "${PROJECT_NAME} pub file: ${pub_file}")
endforeach(pub_file)

#
# The public include directory for this library's clients
#
target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)

# Top-level include directories:
target_include_directories(
  ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                          ${CMAKE_CURRENT_SOURCE_DIR}/src)

#
# Config
#
configure_file("config.h.in" "${PROJECT_BINARY_DIR}/config.h")
include_directories("${PROJECT_BINARY_DIR}")
# if(NOT CMAKE_BUILD_TYPE MATCHES Release) target_compile_definitions(
# ${PROJECT_NAME} PRIVATE # BOOST_ENABLE_ASSERT_HANDLER=1 # ) endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
if(WIN32)
  target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    UNICODE
    _UNICODE
    _CONSOLE
    NOMINMAX
    WIN64
    _WIN64
    WINAPI_FAMILY=WINAPI_FAMILY_DESKTOP_APP
    _WIN32_WINNT=_WIN32_WINNT_WIN10
    _CRT_SECURE_NO_WARNINGS)
  target_compile_options(${PROJECT_NAME} PRIVATE -GR) # Turn on RTTI.
endif()

#
# Parent directory for locally cached repositories of open source software (oss)
#
option(GRANT_INTERNET_ACCESS "Permit CMake to download from the Internet" OFF)
message(STATUS "Access Internet permission: ${GRANT_INTERNET_ACCESS}")
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  get_filename_component(HOME $ENV{USERPROFILE} ABSOLUTE)
else()
  get_filename_component(HOME $ENV{HOME} ABSOLUTE)
endif()
get_filename_component(LOCALLY_CACHED_REPOS_DIR "${HOME}/oss" ABSOLUTE)
if(EXISTS ${LOCALLY_CACHED_REPOS_DIR})
  message(STATUS "Locally cached repositories folder: "
                 ${LOCALLY_CACHED_REPOS_DIR})
else()
  message(WARNING "No local repositories folder: " ${LOCALLY_CACHED_REPOS_DIR})
endif()

#
# Pre-compiled Header
#
# Downloaded external project https://github.com/larsch/cmake-precompiled-header
#
get_filename_component(
  LOCAL_PRECOMPILED_CMAKE
  "${LOCALLY_CACHED_REPOS_DIR}/cmake-precompiled-header/PrecompiledHeader.cmake"
  ABSOLUTE)
if(EXISTS ${LOCAL_PRECOMPILED_CMAKE})
  message(STATUS "PrecompiledHeader.cmake: " ${LOCAL_PRECOMPILED_CMAKE})
  include(${LOCAL_PRECOMPILED_CMAKE})
  target_sources(${PROJECT_NAME} PRIVATE "pch-simple-xml.cpp")
  add_precompiled_header(${PROJECT_NAME} "pch-simple-xml.hpp" SOURCE_CXX
                         "pch-simple-xml.cpp")
endif()

# Make globally available all required external libraries:
add_subdirectory("external")

#
# Import String-Extra git submodule
#
if(NOT TARGET "String-Extra")
  add_subdirectory("String-Extra")
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "String-Extra")

#
# Import OStream-Extra git submodule
#
if(NOT TARGET "OStream-Extra")
  add_subdirectory("OStream-Extra")
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "OStream-Extra")

#
# Import Pseudo-XPath git submodule
#
if(NOT TARGET "Pseudo-XPath")
  add_subdirectory("Pseudo-XPath")
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC "Pseudo-XPath")

#
# Boost library from external
#
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC #
         Boost::boost #
         Boost::filesystem #
)

#
# Unit tests Only build unit tests if this project isn't a submodule in a super-
# project.
#
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  message(STATUS "Building unit tests: ${BUILD_TESTING}")
  if(BUILD_TESTING)
    add_subdirectory("Unit-Tests")
  endif()
endif()
