cmake_minimum_required(VERSION 3.20)
project(aoc)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
aoc.cpp
aoc_header_libs.cpp
)

set(HEADERS
day.hpp
)

foreach(i RANGE 1 20)
    list(APPEND SOURCES "day_${i}.cpp")
    list(APPEND HEADERS "day_${i}.hpp")
endforeach()

set (JLIBCPP_HEADERS
    jcpp_log.hpp
    jcpp_instrumentor.hpp
)

list(TRANSFORM SOURCES PREPEND "src/")
list(TRANSFORM HEADERS PREPEND "src/")
list(TRANSFORM JLIBCPP_HEADERS PREPEND "libraries/jlibc/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmacro-prefix-map=${CMAKE_CURRENT_SOURCE_DIR}=.")

add_executable(aoc ${SOURCES} ${HEADERS} ${JLIBCPP_HEADERS})
target_include_directories(aoc PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(aoc PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libraries/jlibc")

target_compile_options(aoc PUBLIC -Werror -Wall -Wextra -pedantic)

# For clangd
target_compile_options(aoc PUBLIC -Werror=type-limits -Wsuggest-attribute=const -Wsuggest-attribute=noreturn)



if (CMAKE_BUILD_TYPE STREQUAL "debug")
    target_compile_definitions(aoc PUBLIC -DJC_PROFILE)
else ()
    target_compile_definitions(aoc PUBLIC -DJC_NO_TRACE)
    set_target_properties(aoc PROPERTIES UNITY_BUILD ON UNITY_BUILD_MODE BATCH UNITY_BUILD_BATCH_SIZE 32)
endif ()


find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(aoc PUBLIC OpenMP::OpenMP_CXX)
endif()


