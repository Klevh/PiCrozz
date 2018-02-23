cmake_minimum_required(VERSION 3.5)

# Nom du projet

project(PiCrozz_test)

# creation executable

add_executable(
  PiCrozz_test
  # add cpp files here (without main)
  sources/main.cpp
  ../sources/Window.cpp
  ../sources/Errors.cpp
  ../sources/Element.cpp
  ../sources/Program.cpp
  ../sources/Pattern.cpp
  ../sources/Picross.cpp
  ../sources/Colors.cpp
  ../sources/InfoCase.cpp
  ../sources/tinyxml2.cpp
  ../sources/Vec3.cpp
)

# c++ version
set_target_properties(
  PiCrozz_test
  PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)

# trouver les packages

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../cmake")

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

find_package(SDL2 REQUIRED)
IF(SDL2_FOUND)
  MESSAGE(INFO " SDL2 lib found at ${SDL2_LIBRARIES}")
ELSE()
  MESSAGE(ERROR " SDL2 not found")
ENDIF()

find_package(SDL2TTF REQUIRED)
IF(SDLTTF_FOUND)
  MESSAGE(INFO " SDL2_ttf lib found at ${SDLTTF_LIBRARIES}")
ELSE()
  MESSAGE(ERROR " SDL2_ttf not found")
ENDIF()

# enlever la compilation des tests GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# linkage librairie externe

target_link_libraries(
  PiCrozz_test
  # toutes les dependances (glew, glfw, ...)
  # chercher find package cmake
  ${GLEW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  glfw
  -lSDL2
  -lSDL2_ttf
)

# adding include dir

target_include_directories(
  PiCrozz_test
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/../headers/"
  "${CMAKE_CURRENT_SOURCE_DIR}/headers/")

get_target_property(debug PiCrozz_test INTERFACE_INCLUDE_DIRECTORIES)
message("${debug}")

# options de compilations

add_compile_options(
  "-g")

# copie des fichiers dans le build

file(GLOB SHADERS
  "${PROJECT_SOURCE_DIR}/../ressources/*")

file(COPY ${SHADERS} DESTINATION ${CMAKE_BINARY_DIR}/ressources)
