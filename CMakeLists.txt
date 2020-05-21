cmake_minimum_required(VERSION 3.8)

project(oofilesplit)

# add_subdirectory (libhello)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)

add_executable(oofilesplit ${SOURCE_FILES})

# target_link_libraries (oofilesplit LINK_PUBLIC hello)
