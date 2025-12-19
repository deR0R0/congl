cmake_minimum_required(VERSION 3.15)
project(CongL LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(CongL
  src/main.cpp
  src/glad.c
  src/utils/shader_loader.cpp
  src/shapes/square.cpp
)

target_include_directories(CongL PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(CongL PRIVATE
  glfw
  glm::glm
  "-framework OpenGL"
)
