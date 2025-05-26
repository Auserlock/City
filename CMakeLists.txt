cmake_minimum_required(VERSION 3.30)
project(Latte)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(${CMAKE_SOURCE_DIR}/googletest)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)

find_package(Eigen3 3.4 REQUIRED NO_MODULE)

file(GLOB_RECURSE LATTE_HEADER_FILES
        ${CMAKE_SOURCE_DIR}/include/*.hpp
)
file(GLOB_RECURSE CORE_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/core/*.cc
)

add_library(
        LatteCore
        SHARED
        ${CORE_SOURCE_FILES}
        ${LATTE_HEADER_FILES}
)

target_link_libraries(
        LatteCore
        PRIVATE
        Eigen3::Eigen
)

target_include_directories(
        LatteCore
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/core
)

add_executable(
        Latte
        src/main.cpp
)

target_link_libraries(
        Latte
        PRIVATE
        LatteCore
        Eigen3::Eigen
)

target_include_directories(
        Latte
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/core
)

ADD_DEPENDENCIES(Latte LatteCore)
