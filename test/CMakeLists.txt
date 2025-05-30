enable_testing()

find_package(Eigen3 3.4 REQUIRED NO_MODULE)

file(GLOB_RECURSE TEST_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/test/*.cc
)

message(STATUS "Test sources: ${TEST_SOURCE_FILES}")

add_executable(
        CityTest
        ${TEST_SOURCE_FILES}
)

target_include_directories(
        CityTest
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
        CityTest
        PRIVATE
        CityCore
        CityUtil
        Eigen3::Eigen
        gtest
        gtest_main
        pthread
)

add_test(NAME LatteTest COMMAND LatteTest)
