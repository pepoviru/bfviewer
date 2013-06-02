# - Find a ZIPLIB library 
# This module defines
#  ZIPLIB_LIBRARIES, the libraries needed to use ZIPLIB.
#  ZIPLIB_INCLUDE, the includes needed to use ZIPLIB.
#  ZIPLIB_FOUND, If false, do not try to use ZIPLIB.
# also defined, but not for general use are
#  ZIPLIB_LIBRARY, where to find the ZIPLIB library.

SET(ZIPLIB_NAMES ${ZIPLIB_NAMES} ziplib)
FIND_LIBRARY(ZIPLIB_LIBRARY
  NAMES ${ZIPLIB_NAMES}
  PATHS /usr/lib64/ /usr/lib/ /usr/local/lib64 /usr/local/lib
  )

FIND_PATH(ZIPLIB_INCLUDE
  NAMES "ziplib/ziplib.hpp"
  PATH_SUFFIXES include
  )

IF (ZIPLIB_LIBRARY AND ZIPLIB_INCLUDE)
  SET(ZIPLIB_LIBRARIES ${ZIPLIB_LIBRARY})
  SET(ZIPLIB_FOUND "YES")
ELSE (ZIPLIB_LIBRARY AND ZIPLIB_INCLUDE)
  SET(ZIPLIB_FOUND "NO")
ENDIF ()


IF (ZIPLIB_FOUND)
   IF (NOT ZIPLIB_FIND_QUIETLY)
      MESSAGE(STATUS "Found ZIPLIB library: ${ZIPLIB_LIBRARIES}")
   ENDIF (NOT ZIPLIB_FIND_QUIETLY)
ELSE (ZIPLIB_FOUND)
   IF (ZIPLIB_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find a ZIPLIB library")
   ENDIF (ZIPLIB_FIND_REQUIRED)
ENDIF (ZIPLIB_FOUND)

# Deprecated declarations.
MARK_AS_ADVANCED(
  ZIPLIB_LIBRARY
  )
