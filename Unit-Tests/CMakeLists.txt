﻿cmake_minimum_required(VERSION 3.15)
set(TARGET "Unit-Tests")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(${TARGET} "main-unit-tests.cpp" "GlobalFixture.hpp"
                         "GlobalFixture.cpp")
set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 17)
file(
  GLOB SRC_TEST_FILES
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test-*.cpp")
target_sources(${TARGET} PRIVATE ${SRC_TEST_FILES})
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# Debug dump of test src files:
foreach(src_file ${SRC_TEST_FILES})
  message(STATUS "${TARGET} src file: ${src_file}")
endforeach(src_file)

if(WIN32)
  target_compile_definitions(
    ${TARGET}
    PRIVATE
    UNICODE
    _UNICODE
    _CONSOLE
    NOMINMAX
    WIN64
    _WIN64
    WINAPI_FAMILY=WINAPI_FAMILY_DESKTOP_APP
    _WIN32_WINNT=_WIN32_WINNT_WIN10
    _CRT_SECURE_NO_WARNINGS
    _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
  target_compile_options(${TARGET} PRIVATE -GR) # Turn on RTTI.
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
# Pre-compiled Header Downloaded external project
# https://github.com/larsch/cmake-precompiled-header
#
get_filename_component(
  LOCAL_PRECOMPILED_CMAKE
  "${LOCALLY_CACHED_REPOS_DIR}/cmake-precompiled-header/PrecompiledHeader.cmake"
  ABSOLUTE)
if(EXISTS ${LOCAL_PRECOMPILED_CMAKE})
  message(STATUS "PrecompiledHeader.cmake: " ${LOCAL_PRECOMPILED_CMAKE})
  include(${LOCAL_PRECOMPILED_CMAKE})
  target_sources(${TARGET} PRIVATE "pch-unit-tests.cpp")
  add_precompiled_header(${TARGET} "pch-unit-tests.hpp" SOURCE_CXX
                         "pch-unit-tests.cpp")
else()
  message(WARNING "No such repo available: cmake-precompiled-header")
endif()

#
# Import String-Extra git submodule
#
target_link_libraries(${PROJECT_NAME} PRIVATE "String-Extra")

#
# Import OStream-Extra git submodule
#
target_link_libraries(${PROJECT_NAME} PRIVATE "OStream-Extra")

#
# Import Pseudo-XPath git submodule
#
target_link_libraries(${PROJECT_NAME} PUBLIC "Pseudo-XPath")

#
# Import the subjects for unit testing
#
target_link_libraries(${TARGET} PRIVATE "Simple-XML")

#
# Boost
#
add_subdirectory("boost")
target_link_libraries(${TARGET} PRIVATE Boost::boost)
foreach(Boost_Lib $ENV{BOOST_REQUIRED_COMPONENTS})
  target_link_libraries(${TARGET} PRIVATE Boost::${Boost_Lib})
endforeach(Boost_Lib)
if(WIN32)
  add_custom_command(
    TARGET ${TARGET} PRE_BUILD
    COMMAND
      START /MIN "RoboCopy of Boost redist from parent directory" #
      ROBOCOPY /COPY:DAT /NP /R:2 /W:2 "${CMAKE_BINARY_DIR}"
      "${CMAKE_CURRENT_BINARY_DIR}" "boost*dll")
endif(WIN32)

#
# Discover all the unit tests automatically for CTest to execute
#
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
include(Discover.Boost.Test)
boost_discover_tests(${TARGET})
