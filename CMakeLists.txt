#   SPECIFY CMAKE VERSION AND PROJECT NAME
cmake_minimum_required(VERSION 3.27)
project(chisel8 C)

#   SET C STANDARD, COMPILER AND OPENGL VERSION
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_COMPILER clang)
set(OPENGL_VERSION "ES 2.0")

#   DEFINE MAIN BINARY
file(GLOB_RECURSE CORE "src/core/*.c")
file(GLOB_RECURSE EMU "src/emu/*.c")
file(GLOB_RECURSE GUI "src/gui/*.c")
file(GLOB_RECURSE MAIN "src/main.c")
add_executable(${PROJECT_NAME} ${MAIN} ${CORE} ${EMU} ${GUI})

#   ADD SRC AND INCLUDE DIRECTORIES
target_include_directories(${PROJECT_NAME} PRIVATE include)

#   ADD SUBDIRECTORIES AND GIT SUBMODULES
add_subdirectory(external/raylib-stable)    #   CUSTOM RAYLIB REPO - CURRENT STABLE
add_subdirectory(external/raygui-stable)    #   CUSTOM RAYGUI REPO - CURRENT STABLE + CMakeList FILE

#   DEFINE ANGLE BUILD DIRECTORIES AND LIBRARIES
set(ANGLE_LIBRARY_DIR external/angle-lib)
find_library(ANGLE_GLESv2_LIBRARY libGLESv2.dylib PATHS ${ANGLE_LIBRARY_DIR})
find_library(ANGLE_EGL_LIBRARY libEGL.dylib PATHS ${ANGLE_LIBRARY_DIR})

#   TARGET INCLUDE DIRECTORIES FOR RAYLIB, RAYGUI AND ANGLE
target_include_directories(${PROJECT_NAME} PRIVATE external/raylib-stable/src)  #   RAYLIB INCLUDE DIR
target_include_directories(${PROJECT_NAME} PRIVATE external/raygui-stable/src)  #   RAYGUI INCLUDE DIR
target_include_directories(${PROJECT_NAME} PRIVATE external/raygui-stable/styles)

#   LINK LIBRARIES INTO BINARY
target_link_libraries(${PROJECT_NAME} raylib raygui ${ANGLE_EGL_LIBRARY} ${ANGLE_GLESv2_LIBRARY})