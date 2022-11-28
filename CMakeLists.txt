cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0054 NEW)

set(PROJECT_NAME "CornerDetections")
set(CMAKE_CXX_STANDARD 14)

project(${PROJECT_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/Bin)

# Gorgon package required for image processing part
find_package(Gorgon REQUIRED)

add_compile_options(-std=c++14)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Source/Main.cpp)

linkgorgon(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Tests/)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
