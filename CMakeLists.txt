cmake_minimum_required(VERSION 3.20)
project(vengine)

#COMMON
set(CMAKE_CXX_STANDARD 23)

find_package(SDL2)

add_compile_options(-fdiagnostics-color=always -fconcepts-diagnostics-depth=200)

#LIBVENGINE

add_library(vengine SHARED vengine.cpp RTTI.cpp Entity.cpp Archetype.cpp EntityManager.cpp SystemManager.cpp Context.cpp modules/internal/SDL.cpp modules/SDL.cpp)
add_library(vengine_static STATIC vengine.cpp RTTI.cpp Entity.cpp Archetype.cpp EntityManager.cpp SystemManager.cpp Context.cpp modules/internal/SDL.cpp modules/SDL.cpp)
target_include_directories(vengine PUBLIC "libraries/neo" "${SDL2_INCLUDE_DIRS}")
target_compile_definitions(vengine PUBLIC "VENGINE_DEBUG_MESSAGES=1")
target_compile_options(vengine PUBLIC "-mavx2" "-fpic" "-fno-plt" "-Wl,-rpath,.")
target_link_libraries(vengine PRIVATE "${SDL2_LIBRARIES}")

#TESTPROGRAM

add_executable(TestBin TestProgram/main.cpp)
target_link_libraries(TestBin PRIVATE vengine)
target_compile_options(TestBin PUBLIC "-mavx2" "-Wl,-rpath,.")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    MESSAGE(STATUS "Debugs asserts are disabled")
    add_compile_definitions(DEBUG_ASSERTS=1)
endif()

option(VENGINE_DEBUG_MESSAGES "Enables extra debug messages" OFF)