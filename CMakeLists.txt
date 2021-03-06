cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/.cmake/")

project(intel8080)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.72 REQUIRED COMPONENTS filesystem)
find_package(criterion REQUIRED)

include_directories(inc)
include_directories(argparser/src)
include_directories(${Boost_INCLUDE_DIR})

link_libraries(Boost::filesystem)

# Coverage
add_definitions("-fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "--coverage")

# Main
file(GLOB SOURCES src/*.cc)
add_executable(i8080 ${SOURCES} argparser/src/argparser.c)
target_link_libraries(i8080 ${CRITERION_LIBRARIES} ${Boost_LIBRARIES})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/opcodes.csv DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Tests
file(GLOB TEST_SOURCES tests/src/*.cc ${SOURCES})
list(REMOVE_ITEM TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc)
add_executable(test ${TEST_SOURCES} argparser/src/argparser.c)
target_include_directories(test PUBLIC tests/inc)
target_link_libraries(test ${CRITERION_LIBRARIES})
