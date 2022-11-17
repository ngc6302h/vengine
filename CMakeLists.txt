cmake_minimum_required(VERSION 3.20)
project(vengine)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-fdiagnostics-color=always)
add_library(vengine SHARED library.cpp RTTI.cpp Entity.cpp Archetype.cpp EntityManager.cpp SystemManager.cpp Context.cpp)
target_include_directories(vengine PUBLIC "libraries/neo")
target_compile_definitions(vengine PUBLIC "VENGINE_DEBUG_MESSAGES=1")
target_compile_options(vengine PUBLIC "-mavx2" "-fpic" "-fno-plt")

add_executable(TestBin TestProgram/main.cpp)
target_link_libraries(TestBin PRIVATE vengine)
target_compile_options(TestBin PUBLIC "-mavx2")