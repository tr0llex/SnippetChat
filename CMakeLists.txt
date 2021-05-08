cmake_minimum_required(VERSION 3.16)
project(SnippetChat)

configure_file(CMakeLists.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

enable_testing()

set(CMAKE_C_FLAGS "-pedantic -fprofile-arcs -ftest-coverage -Wall -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS "-pedantic -fprofile-arcs -ftest-coverage -Wall -Werror -Wpedantic")
set(CMAKE_CXX_STANDARD 14)

find_package(Wt REQUIRED)
#link_directories("/usr/lib")
#link_directories("/usr/local/lib")

include_directories("${PROJECT_SOURCE_DIR}/project/include")


set(INCLUDE ${PROJECT_SOURCE_DIR}/project/include)
set(SOURCE ${PROJECT_SOURCE_DIR}/project/src)

# adding libs

# end libs

file(GLOB prod_sources
        "${INCLUDE}/*.hpp"
        "${SOURCE}/*.cpp")

add_executable(${PROJECT_NAME} ${prod_sources})

target_link_libraries(${PROJECT_NAME} wthttp wt)

file(GLOB tests "${PROJECT_SOURCE_DIR}/project/tests/*.cpp")
list(REMOVE_ITEM tests "${PROJECT_SOURCE_DIR}/project/tests/main.cpp")

foreach(file ${tests})
    set(name)
    get_filename_component(name ${file} NAME_WE)
    add_executable("${name}_tests"
            ${SOURCE}/${name}.cpp
            ${file}
            "${PROJECT_SOURCE_DIR}/project/tests/main.cpp")
    target_link_libraries("${name}_tests" gtest_main)
    add_test(NAME ${name} COMMAND "${name}_tests")
endforeach()
