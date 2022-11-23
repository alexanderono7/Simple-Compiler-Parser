cmake_minimum_required (VERSION 3.5)
project(project3)

set(QUICK_START_VERSION_MAJOR 1)
set(QUICK_START_VERSION_MINOR 0)

set(SOURCES ${PROJECT_SOURCE_DIR}/parser.cc lexer.cc execute.h inputbuf.cc)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(project3 STATIC ${SOURCES})

enable_testing()

add_subdirectory(tests)