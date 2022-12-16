cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)

project(her)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME} main.cpp)
add_subdirectory(../pio ${CMAKE_BINARY_DIR}/subproject/pio/${PROJECT_NAME})

target_link_libraries (${PROJECT_NAME}
  PRIVATE
  pio
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

