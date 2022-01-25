cmake_minimum_required(VERSION 3.22)

project(pong)

set(simple_VERSION_MAJOR 0)
set(simple_VERSION_MINOR 1)

find_package(SDL2 REQUIRED)

include_directories(pong ${SDL2_INCLUDE_DIRS})

add_executable(
  pong
  ${CMAKE_SOURCE_DIR}/pong.cpp
  ${CMAKE_SOURCE_DIR}/src/Game.cpp
  )

target_include_directories(
  pong PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  )

target_link_libraries(pong ${SDL2_LIBRARIES})
