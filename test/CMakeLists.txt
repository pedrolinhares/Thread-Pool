cmake_minimum_required(VERSION 2.6)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

set(PROJECT_TEST_NAME "runTests")
set(MY_SOURCE_DIR "../src")

find_package(PythonInterp)

FILE(GLOB_RECURSE sourceFiles ${MY_SOURCE_DIR}/*.cpp)
FILE(GLOB_RECURSE testSourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM sourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cpp)

set(SOURCE
    ${sourceFiles}
    ${testSourceFiles}
  )

# get all source reader files
FILE(GLOB_RECURSE headerFiles ${MY_SOURCE_DIR}/*.h)
set(HEADERS ${headerFiles})


# MACRO to get all source dirs
MACRO(HEADER_DIRECTORIES return_list)
    SET(new_list ${headerFiles})
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

# include directories to search for includes
HEADER_DIRECTORIES(sourceDirList)
include_directories(${sourceDirList})

# set flag to use c++11 and test coverage
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -fprofile-arcs -ftest-coverage -fPIC -O0")

# link runTests with what we want to test and the GTest, pthread and gcov library
add_executable(${PROJECT_TEST_NAME} ${SOURCE} ${HEADERS})
target_link_libraries(${PROJECT_TEST_NAME} ${GTEST_LIBRARIES} pthread)
