cmake_minimum_required(VERSION 3.10)

project(Smeagol)

set(C_CXX_SOURCES_DIR "src")
set(C_CXX_HEADERS_DIR "include")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OutputExecutable "${CMAKE_PROJECT_NAME}")


set(SOURCE_CXX_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${C_CXX_HEADERS_DIR})
set(SOURCE_CXX_SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/${C_CXX_SOURCES_DIR})

# Source Files are Curated Here
file(
    GLOB_RECURSE SOURCE_CXX_FILES
    "${SOURCE_CXX_SRC_DIR}/*.cpp"
)

include_directories(${SOURCE_CXX_INCLUDE_DIR})

add_executable(${OutputExecutable} ${SOURCE_CXX_FILES})