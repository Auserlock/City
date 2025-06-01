cmake_minimum_required(VERSION 3.30)
project(City)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_subdirectory(${CMAKE_SOURCE_DIR}/googletest)
#add_subdirectory(${CMAKE_SOURCE_DIR}/test)

option(SKIP_DEBUG_INFO "skip print debug info" OFF)
option(SKIP_ASSERT "skip assert" OFF)
option(SKIP_ARG_CHECK "skip arg check" OFF)

if(SKIP_DEBUG_INFO)
    add_compile_definitions(SKIP_DEBUG_INFO)
endif()

if(SKIP_ASSERT)
    add_compile_definitions(SKIP_ASSERT)
endif()

if(SKIP_ARG_CHECK)
    add_compile_definitions(SKIP_ARG_CHECK)
endif()

find_package(Eigen3 CONFIG REQUIRED)

file(GLOB_RECURSE CITY_HEADER_FILES
        ${CMAKE_SOURCE_DIR}/include/*.hpp
)
file(GLOB_RECURSE CORE_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/core/*.cc
)
file(GLOB_RECURSE UTIL_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/util/*.cc
)

add_library(
        CityUtil
        SHARED
        ${UTIL_SOURCE_FILES}
        ${CITY_HEADER_FILES}
)

target_link_libraries(
        CityUtil
        PRIVATE
        Eigen3::Eigen
)

target_include_directories(
        CityUtil
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

add_library(
        CityCore
        SHARED
        ${CORE_SOURCE_FILES}
        ${CITY_HEADER_FILES}
)

target_link_libraries(
        CityCore
        PRIVATE
        Eigen3::Eigen
)

target_include_directories(
        CityCore
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

add_executable(
        City
        src/main.cpp
)

target_link_libraries(
        City
        PRIVATE
        CityCore
        CityUtil
        Eigen3::Eigen
)

target_include_directories(
        City
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

ADD_DEPENDENCIES(City CityCore)
ADD_DEPENDENCIES(City CityUtil)