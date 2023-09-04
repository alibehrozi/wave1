#
# Copyright (c) 2012, Jared Boone <jared@sharebrained.com>
# Copyright (c) 2013, Benjamin Vernoux <titanmkd@gmail.com>
# Copyright (c) 2013, Michael Ossmann <mike@ossmann.com>
# 
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
# 
#     Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#     Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the 
# 	documentation and/or other materials provided with the distribution.
#     Neither the name of Great Scott Gadgets nor the names of its contributors may be used to endorse or promote products derived from this software
# 	without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Based heavily upon the libftdi cmake setup.

if(BUILD_SHARED_LIBS)
    set(LIBHACKRF_BUILD_SHARED_LIBS_DEFAULT ON)
else()
    set(LIBHACKRF_BUILD_SHARED_LIBS_DEFAULT OFF)
endif()

option(LIBHACKRF_BUILD_SHARED_LIBS_DEFAULT "Build Shared Libraries for libusb" ${LIBHACKRF_BUILD_SHARED_LIBS_DEFAULT})

# Targets
set(c_sources ${CMAKE_CURRENT_SOURCE_DIR}/hackrf.c CACHE INTERNAL "List of C sources")
set(c_headers ${CMAKE_CURRENT_SOURCE_DIR}/hackrf.h CACHE INTERNAL "List of C headers")

if(LIBHACKRF_BUILD_SHARED_LIBS_DEFAULT)
    add_library(hackrf SHARED ${c_sources})
    set_target_properties(hackrf PROPERTIES VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.0 SOVERSION 0)
else()
    add_library(hackrf STATIC ${c_sources})
endif()

set_target_properties(hackrf PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# Dependencies
target_link_libraries(hackrf ${LIBUSB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# For cygwin just force UNIX OFF and WIN32 ON
if( ${CYGWIN} )
  SET(UNIX OFF)
  SET(WIN32 ON)
endif( ${CYGWIN} )

if( ${UNIX} )
    if(LIBHACKRF_BUILD_SHARED_LIBS_DEFAULT)
       install(TARGETS hackrf
               LIBRARY DESTINATION lib${LIB_SUFFIX}
               COMPONENT sharedlibs
               )
   else()
       install(TARGETS hackrf
               ARCHIVE DESTINATION lib${LIB_SUFFIX}
               COMPONENT staticlibs
               )
   endif()
   install(FILES ${c_headers}
           DESTINATION include/${PROJECT_NAME}
           COMPONENT headers
           )
endif( ${UNIX} )

if( ${WIN32} )
    if(LIBHACKRF_BUILD_SHARED_LIBS_DEFAULT)
       install(TARGETS hackrf
               DESTINATION bin
               COMPONENT sharedlibs
               )
   else()
       install(TARGETS hackrf
               DESTINATION bin
               COMPONENT staticlibs
               )
   endif()
   install(FILES ${c_headers}
           DESTINATION include/${PROJECT_NAME}
           COMPONENT headers
           )
endif( ${WIN32} )
