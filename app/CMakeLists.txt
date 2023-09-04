# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.20)
project(wave LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

# Include the libusb
set(libusb usb-1.0)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(libs/libusb)

set(libwave wave)
add_library(${libwave} SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/test.c)

# Include libraries
target_link_libraries(
        ${libwave}
        log)