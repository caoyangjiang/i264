cmake_minimum_required(VERSION 3.11)
project(modules)

add_subdirectory(common)
add_subdirectory(encoder)

add_library(${CMAKE_PROJECT_NAME} INTERFACE)

target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE i264_common)
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE i264_encoder)

install(TARGETS ${CMAKE_PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        ARCHIVE DESTINATION lib COMPONENT install-app
        LIBRARY DESTINATION lib COMPONENT install-app
        RUNTIME DESTINATION bin COMPONENT install-app)

export(TARGETS ${CMAKE_PROJECT_NAME}
       APPEND
       FILE ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Targets.cmake)
