# - Find a ECGLIB library 
# This module defines
#  ECGLIB_LIBRARIES, the libraries needed to use ECGLIB.
#  ECGLIB_INCLUDE, the includes needed to use ECGLIB.
#  ECGLIB_FOUND, If false, do not try to use ECGLIB.
# also defined, but not for general use are
#  ECGLIB_LIBRARY, where to find the ECGLIB library.

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	SET(ECGLIB_NAMES ${ECGLIB_NAMES} ecglib)
else()
	SET(ECGLIB_NAMES ${ECGLIB_NAMES} ecglib-dbg)
endif()

FIND_LIBRARY(ECGLIB_LIBRARY
  NAMES ${ECGLIB_NAMES}
  PATHS /usr/lib64/ /usr/lib/ /usr/local/lib64 /usr/local/lib
  )

FIND_PATH(ECGLIB_INCLUDE
  NAMES "ecglib/config.hpp"
  PATH_SUFFIXES include
  )

IF (ECGLIB_LIBRARY AND ECGLIB_INCLUDE)
  SET(ECGLIB_LIBRARIES ${ECGLIB_LIBRARY})
  SET(ECGLIB_FOUND "YES")
ELSE (ECGLIB_LIBRARY AND ECGLIB_INCLUDE)
  SET(ECGLIB_FOUND "NO")
ENDIF ()


IF (ECGLIB_FOUND)
   IF (NOT ECGLIB_FIND_QUIETLY)
      MESSAGE(STATUS "Found ECGLIB library: ${ECGLIB_LIBRARIES}")
   ENDIF (NOT ECGLIB_FIND_QUIETLY)
ELSE (ECGLIB_FOUND)
   IF (ECGLIB_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find a ECGLIB library")
   ENDIF (ECGLIB_FIND_REQUIRED)
ENDIF (ECGLIB_FOUND)

# Deprecated declarations.
MARK_AS_ADVANCED(
  ECGLIB_LIBRARY
  )
