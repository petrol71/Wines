# stop if cmake version below 3.5
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and supported languages
project(Wines)

message("PROJECT_NAME              ${PROJECT_NAME}")
message("PROJECT_SOURCE_DIR        ${PROJECT_SOURCE_DIR}")
message("CMAKE_SOURCE_DIR          ${CMAKE_SOURCE_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}")
message("PROJECT_BINARY_DIR        ${PROJECT_BINARY_DIR}")
message("CMAKE_BINARY_DIR          ${CMAKE_BINARY_DIR}")
message("CMAKE_CURRENT_BINARY_DIR  ${CMAKE_CURRENT_BINARY_DIR}")
message("CMAKE_INSTALL_PREFIX      ${CMAKE_INSTALL_PREFIX}")

# detect operating system and host processor
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

project (Wines)

add_executable(winesApp wines.c)
#set_target_properties(winesApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")