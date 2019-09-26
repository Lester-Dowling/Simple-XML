cmake_minimum_required (VERSION 3.14)
project ("Simple-XML-2019")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(GRANT_INTERNET_ACCESS "Permit CMake to download from the Internet" OFF)
message(STATUS "Access Internet permission: ${GRANT_INTERNET_ACCESS}")
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable (Simple-XML "Simple-XML.cpp" "Simple-XML.h")

#
# Parent directory for locally cached repositories of open source software (oss)
#
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  get_filename_component(HOME $ENV{USERPROFILE} ABSOLUTE)
else()
  get_filename_component(HOME $ENV{HOME} ABSOLUTE)
endif()
get_filename_component(LOCALLY_CACHED_REPOS_DIR "${HOME}/oss" ABSOLUTE)
if(EXISTS ${LOCALLY_CACHED_REPOS_DIR})
  message(STATUS "Locally cached repositories folder: " ${LOCALLY_CACHED_REPOS_DIR})
else()
  message(WARNING "No local repositories folder: " ${LOCALLY_CACHED_REPOS_DIR})
endif()


# Make globally available all required external libraries:
add_subdirectory ("external")


#
# Unit tests
#
# include(CTest)
# enable_testing()
add_subdirectory ("Unit-Tests")
