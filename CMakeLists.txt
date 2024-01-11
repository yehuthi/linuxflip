cmake_minimum_required(VERSION 3.1...3.28)
project(LinuxFlip VERSION 0.1)

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBINPUT REQUIRED libinput)
pkg_search_module(LIBUDEV REQUIRED libudev)

add_executable(linuxflip main.c)
target_include_directories(linuxflip PRIVATE ${LIBINPUT_INCLUDE_DIRS} ${LIBUDEV_INCLUDE_DIRS})
target_link_libraries(linuxflip ${LIBINPUT_LIBRARIES} ${LIBUDEV_LIBRARIES})
