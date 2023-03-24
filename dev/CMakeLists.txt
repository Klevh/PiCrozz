cmake_minimum_required(VERSION 3.5)

# debug flag, to comment for release

set(CMAKE_BUILD_TYPE Debug)

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
  sources/Picross.cpp
  sources/Colors.cpp
  sources/InfoCase.cpp
  sources/tinyxml2.cpp
  sources/Vec3.cpp
  sources/OperationsQueue.cpp
)

# c++ version
set_target_properties(
  PiCrozz
  PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)

# trouver les packages

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

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

find_package(Boost REQUIRED)
IF(Boost_FOUND)
  MESSAGE(INFO " Boost lib found at ${Boost_LIBRARIES}")
ELSE()
  MESSAGE(ERROR " Boost not found")
ENDIF()

find_package(Boost COMPONENTS filesystem REQUIRED)
IF(Boost_FILESYSTEM_FOUND)
  MESSAGE(INFO " Boost filesystem lib found at ${Boost_FILESYSTEM_LIBRARIES}")
ELSE()
  MESSAGE(ERROR " Boost filesystem not found")
ENDIF()

find_package(Boost COMPONENTS system REQUIRED)
IF(Boost_SYSTEM_FOUND)
  MESSAGE(INFO " Boost system lib found at ${Boost_FILESYSTEM_LIBRARIES}")
ELSE()
  MESSAGE(ERROR " Boost system not found")
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
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  glfw
  -lSDL2
  -lSDL2_ttf
)

# adding include dir

target_include_directories(
  PiCrozz
  PUBLIC headers/
  ${Boost_INCLUDE_DIRS})

# options de compilations

add_compile_options(
  "-g")

# copie des fichiers dans le build

file(GLOB SHADERS
  "${PROJECT_SOURCE_DIR}/ressources/*")

file(COPY ${SHADERS} DESTINATION ${CMAKE_BINARY_DIR}/ressources)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/ressources/Grids/Saves)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/ressources/Grids/Finished)
