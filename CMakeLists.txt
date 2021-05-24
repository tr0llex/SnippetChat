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
find_package( Threads )

enable_testing()

#set(CMAKE_C_FLAGS "-pthread -pedantic -fprofile-arcs -ftest-coverage -Wall -Werror -Wpedantic")
#set(CMAKE_CXX_FLAGS "-pthread -pedantic -fprofile-arcs -ftest-coverage -Wall -Werror -Wpedantic")
set(CMAKE_CXX_STANDARD 17)

find_package(Wt REQUIRED)

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

add_library(Auth STATIC
        ${PROJECT_SOURCE_DIR}/hash/md5.h
        ${PROJECT_SOURCE_DIR}/hash/md5.cpp
        ${INCLUDE}/Auth.hpp
        ${SOURCE}/Auth.cpp)

add_library(CompilationManager STATIC
        ${INCLUDE}/CompilationManager.hpp
        ${INCLUDE}/Compilation.hpp
        ${SOURCE}/CompilationManager.cpp
        ${SOURCE}/Compilation.cpp)

add_library(InMemoryDb STATIC
        ${INCLUDE}/InMemoryDb.hpp
        ${SOURCE}/InMemoryDb.cpp)
# end libs

file(GLOB prod_sources
        "${INCLUDE}/ChatApplication.hpp"
        "${INCLUDE}/ChatEvent.hpp"
        "${INCLUDE}/ChatServer.hpp"
        "${INCLUDE}/ChatWidget.hpp"
        "${INCLUDE}/CodeWidget.hpp"
        "${INCLUDE}/DialogueWidget.hpp"
        "${INCLUDE}/MessageWidget.hpp"
        "${SOURCE}/ChatApplication.cpp"
        "${SOURCE}/ChatEvent.cpp"
        "${SOURCE}/ChatServer.cpp"
        "${SOURCE}/ChatWidget.cpp"
        "${SOURCE}/CodeWidget.cpp"
        "${SOURCE}/MessageWidget.cpp"
        "${SOURCE}/DialogueWidget.cpp"
        "${SOURCE}/main.cpp")

add_executable(${PROJECT_NAME} ${prod_sources})


target_link_libraries(${PROJECT_NAME} wthttp wt MainDb cassandra Auth CompilationManager InMemoryDb Models ${CMAKE_THREAD_LIBS_INIT})


file(GLOB tests "${PROJECT_SOURCE_DIR}/project/tests/*.cpp")
list(REMOVE_ITEM tests "${PROJECT_SOURCE_DIR}/project/tests/main.cpp")


foreach(file ${tests})
    set(name)
    get_filename_component(name ${file} NAME_WE)
    add_executable("${name}_tests"
            ${SOURCE}/${name}.cpp
            ${file}
            "${PROJECT_SOURCE_DIR}/project/tests/main.cpp" hash/md5.cpp)
    target_link_libraries("${name}_tests" gtest_main gmock_main ${name} Models )
    add_test(NAME ${name} COMMAND "${name}_tests")
endforeach()
