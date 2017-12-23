cmake_minimum_required(VERSION 3.5)

# Nom du projet

project(PiCrozz)

# creation executable

add_executable(
  PiCrozz
  # add cpp files here (without main)
  sources/main.cpp
  sources/Window.cpp
  sources/Errors.cpp
  sources/Element.cpp
  sources/Program.cpp
  sources/Pattern.cpp
  sources/grille.cpp
  sources/tinyxml2.cpp
  sources/Vec3.cpp
)

# c++ version
set_target_properties(
  PiCrozz
  PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)

# trouver les packages
find_package(OpenGL REQUIRED)
IF(OpenGL_FOUND)
  MESSAGE(INFO " OpenGL lib found at ${OpenGL_LIBRARIES}")
ELSE()
  MESSAGE(ERROR " OpenGL not found")
ENDIF()

find_package(GLEW REQUIRED)
IF(GLEW_FOUND)
  MESSAGE(INFO " glew lib found at ${GLEW_LIBRARIES}")
ELSE()
  MESSAGE(ERROR " glew not found")
ENDIF()

find_package(glfw3 REQUIRED)
IF(glfw3_FOUND)
  MESSAGE(INFO " glfw3 lib found at ${glfw3_LIBRARIES}")
ELSE()
  MESSAGE(ERROR " glfw3 not found")
ENDIF()

# enlever la compilation des tests GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# linkage librairie externe

target_link_libraries(
  PiCrozz
  # toutes les dependances (glew, glfw, ...)
  # chercher find package cmake
  ${GLEW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  glfw
)

# adding include dir

target_include_directories(
  PiCrozz
  PUBLIC headers/)

# options de compilations

add_compile_options(
  "-g")

# copie des fichiers dans le build

file(GLOB SHADERS
  "${PROJECT_SOURCE_DIR}/ressources/*")

file(COPY ${SHADERS} DESTINATION ${CMAKE_BINARY_DIR}/ressources)
