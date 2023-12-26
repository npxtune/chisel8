#   SPECIFY CMAKE VERSION AND PROJECT NAME
cmake_minimum_required(VERSION 3.27)
project(chisel8 C)

#   SET C STANDARD, COMPILER AND OPENGL VERSION
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_COMPILER clang)
option(ANGLE "Enable ANGLE support")

if (NOT CMAKE_BUILD_TYPE)
    MESSAGE("Defaulting to Build type Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif ()

##   CONFIGURE DEFAULT OPENGL VERSION
if (APPLE)
    if (ANGLE)
        set(OPENGL_VERSION "ES 2.0")
        set(GRAPHICS "GRAPHICS_API_OPENGL_ES2")
    else ()
        set(OPENGL_VERSION "3.3")
        set(GRAPHICS "GRAPHICS_API_OPENGL_33")
    endif ()
else ()
    set(OPENGL_VERSION "4.3")
    set(GRAPHICS "GRAPHICS_API_OPENGL_43")
endif ()


#   OBTAIN GIT REF STRING
execute_process(
        COMMAND git describe --abbrev=7 --dirty --always --tags
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_REF
        OUTPUT_STRIP_TRAILING_WHITESPACE
)


#   SET GIT REF STRING AS VERSION NUMBER IN VERSION.H
if (NOT GIT_REF)
    set(GIT_REF "N/A")
endif ()
set(VERSION "${GIT_REF}")
configure_file(${CMAKE_SOURCE_DIR}/include/gui/version.h.in
        ${CMAKE_SOURCE_DIR}/include/gui/version.h
        @ONLY
)


#   DEFINE SOURCE FILE LOCATIONS + ADD THEM
file(GLOB_RECURSE CORE "src/core/*.c"
        GUI "src/gui/*.c"
        MAIN "src/main.c"
)
add_executable(${PROJECT_NAME}
        ${MAIN}
        ${CORE}
        ${GUI}
)


#   ADD INCLUDE DIRS
target_include_directories(${PROJECT_NAME} PRIVATE include)


#   ADD GIT SUBMODULES
add_subdirectory(external/raylib-stable)     #   CUSTOM RAYLIB REPOS - CURRENT STABLE
add_subdirectory(external/raygui-stable)


#   IF ANGLE:   DEFINE ANGLE DIRECTORY AND LIBRARIES
if (ANGLE)
    set(ANGLE_LIBRARY_DIR external/angle-lib)
    find_library(ANGLE_GLESv2_LIBRARY libGLESv2.dylib PATHS ${ANGLE_LIBRARY_DIR}
            ANGLE_EGL_LIBRARY libEGL.dylib PATHS ${ANGLE_LIBRARY_DIR})
endif ()


#   TARGET INCLUDE DIRECTORIES FOR RAYLIB, RAYGUI AND ANGLE
target_include_directories(${PROJECT_NAME} PRIVATE external/raylib-stable/src   #   RAYLIB INCLUDE DIR
        ${PROJECT_NAME} PRIVATE external/raygui-stable/src                      #   RAYGUI INCLUDE DIR
        ${PROJECT_NAME} PRIVATE external/raygui-stable/styles                   #   RAYGUI STYLES DIR
)


#   IF ANGLE:   LINK LIBRARIES INTO BINARY WITH ANGLE
if (ANGLE)
    target_link_libraries(${PROJECT_NAME}
            raylib
            raygui
            ${ANGLE_EGL_LIBRARY}
            ${ANGLE_GLESv2_LIBRARY}
    )


    #   IF NOT ANGLE:  LINK LIBRARIES INTO BINARY
else ()
    target_link_libraries(${PROJECT_NAME}
            raylib
            raygui
    )
endif ()

#   SHOW GITHUB REF STRING
MESSAGE("-- GIT: ${GIT_REF}")


#   WARNINGS IF NO ANGLE MACOS
if (APPLE AND NOT CMAKE_SYSTEM STRLESS "Darwin-18.0.0" AND NOT ANGLE)
    MESSAGE(AUTHOR_WARNING "Please consider using ANGLE starting with macOS 10.14 (Mojave)!")
endif ()