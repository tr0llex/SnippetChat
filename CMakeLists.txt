cmake_minimum_required(VERSION 3.16)
project(messenger)

configure_file(CMakeLists.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

set(CMAKE_C_FLAGS "-pedantic -Wall")
set(CMAKE_CXX_FLAGS "-pedantic -Wall")

enable_testing()


include_directories("${PROJECT_SOURCE_DIR}/project/include")


set(INCLUDE ${PROJECT_SOURCE_DIR}/project/include)
set(SOURCE ${PROJECT_SOURCE_DIR}/project/src)

# adding libs
add_library(Models STATIC
        ${INCLUDE}/Models.hpp
        ${SOURCE}/Models.cpp)

add_library(MainDb STATIC
        ${INCLUDE}/MainDb.hpp
        ${INCLUDE}/IMainDb.hpp
        ${SOURCE}/MainDb.cpp)
# end libs

file(GLOB prod_sources
        "${INCLUDE}/*.hpp"
        "${SOURCE}/main.cpp")

add_executable(main.out ${SOURCE}/main.cpp)
target_link_libraries(main.out MainDb Models cassandra)

file(GLOB tests "${PROJECT_SOURCE_DIR}/project/tests/*.cpp")
list(REMOVE_ITEM tests "${PROJECT_SOURCE_DIR}/project/tests/main.cpp")

foreach(file ${tests})
    set(name)
    get_filename_component(name ${file} NAME_WE)
    add_executable("${name}_tests"
            ${SOURCE}/${name}.cpp
            ${file}
            "${PROJECT_SOURCE_DIR}/project/tests/main.cpp")
    target_link_libraries("${name}_tests" gtest_main gmock_main cassandra)
    add_test(NAME ${name} COMMAND "${name}_tests")
endforeach()
