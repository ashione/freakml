cmake_minimum_required (VERSION 2.8)

project (freakml)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")  

include_directories("/usr/include/eigen3")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(freakml SHARED ${SRC_LIST})
target_link_libraries(freakml pthread glog gflags)

function(addTestExe name)
    string(REGEX REPLACE ".*/.*/(.*).cpp" "\\1.bin" bin_name ${name})
    message("bin name ${bin_name}")
    add_executable(${bin_name} ${name})
    target_link_libraries(${bin_name} freakml)
endfunction(addTestExe)

file(GLOB_RECURSE TEST_LIST "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

foreach(test_name ${TEST_LIST})
    message("test name ${test_name}")
    addTestExe(${test_name})
endforeach(test_name)
