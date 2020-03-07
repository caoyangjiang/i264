cmake_minimum_required(VERSION 3.11)

project(i264_common)

if(MSVC)
  set(DLLIMPORT "__declspec(dllimport)")
  set(DLLEXPORT "__declspec(dllexport)")
  set(DISABLE "__pragma(warning(push,0))")
  set(ENABLE "__pragma(warning(pop))")
endif()

set(PROJECT_SRCS ${PROJECT_SOURCE_DIR}/src/common.cpp)

set(PROJECT_INCS ${PROJECT_SOURCE_DIR}/include/i264/common/common.h)

source_group("include" FILES ${PROJECT_INCS})
source_group("src" FILES ${PROJECT_SRCS})

add_library(${PROJECT_NAME} SHARED ${PROJECT_SRCS} ${PROJECT_INCS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_compile_definitions(
  ${PROJECT_NAME} PUBLIC WINDOWS_DISABLE_ALL_WARNING=${DISABLE}
  WINDOWS_ENABLE_ALL_WARNING=${ENABLE})

target_compile_definitions(${PROJECT_NAME} INTERFACE
                           I264_COMMON_DLL=${DLLIMPORT})

target_compile_definitions(${PROJECT_NAME} PRIVATE I264_COMMON_DLL=${DLLEXPORT})

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER
                                             ${CMAKE_PROJECT_NAME}/modules)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${CMAKE_PROJECT_NAME}Targets
  ARCHIVE DESTINATION lib COMPONENT install-app
  LIBRARY DESTINATION lib COMPONENT install-app
  RUNTIME DESTINATION bin COMPONENT install-app)

install(FILES ${PROJECT_SOURCE_DIR}/include/i264/common/common.h
        DESTINATION include/i264/common)

export(TARGETS ${PROJECT_NAME} APPEND
       FILE ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Targets.cmake)
