cmake_minimum_required(VERSION 3.1...3.28)
project(LinuxFlip VERSION 0.1.2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

find_library(LIBINPUT input REQUIRED)
find_library(LIBUDEV  udev  REQUIRED)

add_library(linuxflip src/linuxflip.c)
target_include_directories(linuxflip PUBLIC include)
target_link_libraries(linuxflip ${LIBINPUT} ${LIBUDEV})

add_executable(exe main.c)
set_property(TARGET exe PROPERTY OUTPUT_NAME linuxflip)
target_compile_definitions(exe PRIVATE LINUXFLIP_VERSION=${PROJECT_VERSION})
target_link_libraries(exe linuxflip)
