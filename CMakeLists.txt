cmake_minimum_required(VERSION 3.1...3.28)
project(LinuxFlip VERSION 0.1.2)

find_library(LIBINPUT input REQUIRED)
find_library(LIBUDEV  udev  REQUIRED)

add_executable(linuxflip main.c cli.c)
target_compile_definitions(linuxflip PRIVATE LINUXFLIP_VERSION=${PROJECT_VERSION})
target_link_libraries(linuxflip ${LIBINPUT} ${LIBUDEV})
