cmake_minimum_required(VERSION 3.11)

project(common_test)

set(NCS ${PROJECT_SOURCE_DIR}/include/transform_test.h
    ${PROJECT_SOURCE_DIR}/include/common_test.h)

set(SRCS
    ${PROJECT_SOURCE_DIR}/src/transform_test.cpp
    ${PROJECT_SOURCE_DIR}/src/common_test.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp)

find_package(GTest CONFIG REQUIRED)

add_executable(common_test ${SRCS} ${INCS})

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

target_link_libraries(common_test i264_common GTest::gtest_main)

set_property(TARGET common_test
             PROPERTY FOLDER ${CMAKE_PROJECT_NAME}/unit-test)
