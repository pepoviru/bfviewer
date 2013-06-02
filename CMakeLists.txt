###
# Copyright (c) 2009, Paul Gideon Dann
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
###

cmake_minimum_required(VERSION 2.8.6)

EXEC_PROGRAM( git ARGS describe --abbrev=0 --tags OUTPUT_VARIABLE VER_STR)
string(REPLACE "." ";" VER_LIST ${VER_STR})
LIST(GET VER_LIST 0 VER_MAJOR)
LIST(GET VER_LIST 1 VER_MINOR)
LIST(GET VER_LIST 2 VER_PATCH)
LIST(LENGTH VER_LIST len)

if ( len GREATER 3)
	LIST(GET VER_LIST 3 VER_BUILD)
	SET(VER_PATCH ${VER_PATCH}.${VER_BUILD})
else()
	SET(VER_BUILD "")
endif()
set(BFVIEWER_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_PATCH}")
set(BFVIEWER_VERSION_MAJOR ${VER_MAJOR})
set(BFVIEWER_VERSION_MINOR ${VER_MINOR})
set(BFVIEWER_VERSION_PATCH ${VER_PATCH})

CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/src/config.hpp.cmake ${CMAKE_SOURCE_DIR}/src/config.hpp )

project(bfviewer)
set(PROJECT_LONGNAME "ECGViewer")
set(PROJECT_VERSION ${BFVIEWER_VERSION})

# If no build type is specified, default to "Release"
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "None Debug Release RelWithDebInfo MinSizeRel"
      FORCE)
endif (NOT CMAKE_BUILD_TYPE)

# Configure Qt
find_package(Qt4 REQUIRED QtCore QtGui QtSvg)
include(${QT_USE_FILE})
set(CMAKE_AUTOMOC true)

if("${WINDRES}" STREQUAL "")
	SET(WINDRES "windres.exe")
endif()

if (WIN32)
  # Stock Qt is built using the C++03 standard. To avoid potential breakage,
  # let's make this explicit.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++03")
  # Since Qt doesn't use shared libstdc++, let's avoid it too.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
  # Windows installer
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/win/installer.nsi.cmake
                 ${CMAKE_CURRENT_BINARY_DIR}/installer.nsi @ONLY)
endif (WIN32)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build_aux/cmake/Modules/")

set(BFVIEWER_LIBRARIES "" CACHE INTERNAL "BFVIEWER LIBRARIES")

# Boost
if( "$ENV{BOOST_ROOT}" STREQUAL "")
        message(STATUS "environment variable BOOST_ROOT not defined searching in default paths")
else()
        message(STATUS "BOOST_ROOT set to: $ENV{BOOST_ROOT}, so skipping system path")
        set(Boost_NO_SYSTEM_PATHS TRUE)
endif()

include(FindBoost)
find_package(Boost COMPONENTS date_time filesystem system regex
program_options iostreams serialization)

if(Boost_FOUND)
        message(STATUS "Boost_MAJOR_VERSION = ${Boost_MAJOR_VERSION}")
        message(STATUS "Boost_MINOR_VERSION = ${Boost_MINOR_VERSION}")
        message(STATUS "Boost_INCLUDE_DIR = ${Boost_INCLUDE_DIR}")
        message(STATUS "Boost_LIB_DIR = ${Boost_LIBRARY_DIRS}")

        # FIXME Check version has fs3
        include_directories(${Boost_INCLUDE_DIR})
        set(BFVIEWER_LIBRARIES "${Boost_LIBRARIES}" CACHE INTERNAL "BFVIEWER LIBRARIES")
else()
        message(FATAL_ERROR "Boost not found")
endif()

include(FindECGLIB)
if(ECGLIB_FOUND)
        message(STATUS "ECGLIB libraries: ${ECGLIB_LIBRARIES}")
        message(STATUS "ECGLIB includes: ${ECGLIB_INCLUDE}")

        set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};${ECGLIB_LIBRARIES}" CACHE INTERNAL "BFVIEWER LIBRARIES")
        include_directories(${ECGLIB_INCLUDE})
else()
        message(FATAL_ERROR "ECGLIB not found")
endif()

# Armadillo
include(FindArmadillo)
if(ARMADILLO_FOUND)
	message(STATUS "Armadillo_MAJOR_VERSION = ${ARMADILLO_VERSION_MAJOR}")
	message(STATUS "Armadillo_MINOR_VERSION = ${ARMADILLO_VERSION_MINOR}")
	include_directories(include ${ARMADILLO_INCLUDE_DIRS})
	if(UNIX)
		set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};${ARMADILLO_LIBRARY}" CACHE INTERNAL "BFVIEWER LIBRARIES")
	endif()
else()
	message(FATAL_ERROR "Armadillo not found")
endif()

if( "$ENV{BLAS_LAPACK}" STREQUAL "")
	include(FindBLAS)
	include(FindLAPACK)
	if(BLAS_FOUND AND LAPACK_FOUND)
		message(STATUS "Blas libraries = ${BLAS_LIBRARIES}")
		message(STATUS "Lapack libraries = ${LAPACK_LIBRARIES}")

		set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};${LAPACK_LIBRARIES}" CACHE INTERNAL "BFVIEWER LIBRARIES")
	else()
		message(FATAL_ERROR "Lapack/Blas not found")
	endif()
else()
	message(STATUS "Skipping autodetect for lapack/atlas and use the following libraries: ${$ENV{LAPACK_ROOT}}")
	set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};$ENV{BLAS_LAPACK}" CACHE INTERNAL "BFVIEWER LIBRARIES")
endif()

# Sigproc
include(FindSIGPROC)
if(SIGPROC_FOUND)
        message(STATUS "SIGPROC libraries: ${SIGPROC_LIBRARIES}")
        message(STATUS "SIGPROC includes: ${SIGPROC_INCLUDE}")

        set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};${SIGPROC_LIBRARIES}" CACHE INTERNAL "BFVIEWER LIBRARIES")
        include_directories(${SIGPROC_INCLUDE})
else()
        message(FATAL_ERROR "SIGPROC not found")
endif()

include(FindGSL)
if(GSL_FOUND)
        message(STATUS "GSL libraries: ${GSL_LIBRARIES}")
        message(STATUS "GSL includes: ${GSL_INCLUDE}")

        set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};${GSL_LIBRARIES}" CACHE INTERNAL "BFVIEWER LIBRARIES")
        include_directories(${GSL_INCLUDE})
else()
        message(FATAL_ERROR "GSL not found")
endif()

include(FindZLIB)
if(ZLIB_FOUND)
        message(STATUS "ZLIB libraries: ${ZLIB_LIBRARIES}")
        message(STATUS "ZLIB includes: ${ZLIB_INCLUDE}")

        set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};${ZLIB_LIBRARIES}" CACHE INTERNAL "BFVIEWER LIBRARIES")
        include_directories(${ZLIB_INCLUDE})
else()
        message(FATAL_ERROR "ZLIB not found")
endif()

include(FindZIPLIB)
if(ZIPLIB_FOUND)
        message(STATUS "ZIPLIB libraries: ${ZIPLIB_LIBRARIES}")
        message(STATUS "ZIPLIB includes: ${ZIPLIB_INCLUDE}")

        set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};${ZIPLIB_LIBRARIES}" CACHE INTERNAL "BFVIEWER LIBRARIES")
        include_directories(${ZIPLIB_INCLUDE})
else()
        message(FATAL_ERROR "BFVIEWER not found")
endif()

include(FindQWT)
if(QWT_FOUND)
        message(STATUS "QWT libraries: ${QWT_LIBRARIES}")
        message(STATUS "QWT includes: ${QWT_INCLUDE}")

        set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};${QWT_LIBRARIES}" CACHE INTERNAL "BFVIEWER LIBRARIES")
        include_directories(${QWT_INCLUDE})
else()
        message(FATAL_ERROR "QWT not found")
endif()

# Ticpp
include(FindTICPP)
if(TICPP_FOUND)
	message(STATUS "TICPP libraries: ${TICPP_LIBRARIES}")
	message(STATUS "TICPP includes: ${TICPP_INCLUDE}")

	set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};${TICPP_LIBRARIES}" CACHE INTERNAL "BFVIEWER LIBRARIES")
include_directories(${TICPP_INCLUDE})
else()
	message(FATAL_ERROR "TICPP not found")
endif()

include(FindLibLZMA)
if(LIBLZMA_FOUND)
	message(STATUS "LZMA libraries: ${LIBLZMA_LIBRARIES}")
	message(STATUS "LZMA includes: ${LIBLZMA_INCLUDE}")

	set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};${LIBLZMA_LIBRARIES}" CACHE INTERNAL "BFVIEWER LIBRARIES")
include_directories(${LIBLZMA_INCLUDE})
else()
	message(FATAL_ERROR "LZMA not found")
endif()

include(FindLCMS)
if(LCMS_FOUND)
	message(STATUS "LCMS libraries: ${LCMS_LIBRARIES}")
	message(STATUS "LCMS includes: ${LCMS_INCLUDE_DIRS}")

	set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};${LCMS_LIBRARIES}" CACHE INTERNAL "BFVIEWER LIBRARIES")
include_directories(${LCMS_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "LCMS not found")
endif()

#######################################
# WFDB
include(FindCURL)
if(CURL_FOUND)
        message(STATUS "CURL libraries: ${CURL_LIBRARIES}")
        message(STATUS "CURL includes: ${CURL_INCLUDE}")

        set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};${CURL_LIBRARIES}" CACHE INTERNAL "BFVIEWER LIBRARIES")
        include_directories(${CURL_INCLUDE})

        include(FindWFDB)
        if(WFDB_FOUND)
                message(STATUS "WFDB libraries: ${WFDB_LIBRARIES}")
                message(STATUS "WFDB includes: ${WFDB_INCLUDE}")

                set(BFVIEWER_LIBRARIES "${BFVIEWER_LIBRARIES};${WFDB_LIBRARIES}" CACHE INTERNAL "BFVIEWER LIBRARIES")
                include_directories(${WFDB_INCLUDE})
        endif()
endif()


add_subdirectory(src)

message(${BFVIEWER_LIBRARIES})
