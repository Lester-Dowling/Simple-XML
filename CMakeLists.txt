cmake_minimum_required(VERSION 3.15)
project(
  "Simple-XML"
  VERSION 0.5.0
  DESCRIPTION
    "Abridged XML grammar to parse simple XML files like from MS Excel.")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

#
# This is a static library project
#
add_library(${PROJECT_NAME} STATIC)
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

# Symbol visibility
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

# Private include directories:
target_include_directories(
  ${PROJECT_NAME} #
  PRIVATE #
          ${CMAKE_CURRENT_SOURCE_DIR}/src)

#
# Config
#
# configure_file("config.h.in" "${PROJECT_BINARY_DIR}/config.h")
# include_directories("${PROJECT_BINARY_DIR}") if(NOT CMAKE_BUILD_TYPE MATCHES
# Release) target_compile_definitions( ${PROJECT_NAME} PRIVATE #
# BOOST_ENABLE_ASSERT_HANDLER=1 # ) endif()

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
# Boost
#
if(NOT DEFINED ENV{BOOST_REQUIRED_COMPONENTS})
  set(ENV{BOOST_REQUIRED_COMPONENTS} "boost")
endif()
function(boost_add_component)
  foreach(boost_component ${ARGV})
    set(ENV{BOOST_REQUIRED_COMPONENTS}
        "$ENV{BOOST_REQUIRED_COMPONENTS};${boost_component}")
  endforeach(boost_component)
endfunction()

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC #
         Boost::boost #
         Boost::filesystem #
         Boost::date_time #
         Boost::regex #
)
boost_add_component(filesystem date_time regex)

#
# External Libraries
#
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

# Build unit tests only if this project isn't a submodule in a super-project.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  message(STATUS "Building unit tests: ${BUILD_TESTING}")
  if(BUILD_TESTING)
    boost_add_component(filesystem date_time regex unit_test_framework)
  endif(BUILD_TESTING)
endif()

#
# Unit tests
#
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory("Unit-Tests")
endif()
