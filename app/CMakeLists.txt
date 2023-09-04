# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.20)
project(wave LANGUAGES C)

set(THREADS_PTHREAD_ARG "-pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

# Include the libusb
set(libusb usb-1.0)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(libs/libusb)

# Include the libhackrf
set(libhackrf hackrf-static)
set(LIBUSB_LIBRARIES ${libusb})
set(LIBUSB_INCLUDE_DIR libs/libusb)
add_subdirectory(libs/libhackrf)

# Include the libfftw3 in single precision mode (libfftw3f)
set(libfftw3 fftw3f)

set(BUILD_TESTS OFF)
set(ENABLE_FLOAT ON)
set(ENABLE_THREADS ON)
set(DISABLE_FORTRAN ON)
add_subdirectory(libs/libfftw3)

set(libwave wave)
add_library(${libwave} SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/test.c)

# Include libraries
target_link_libraries(
        ${libwave}
        log)