# - Find a GSL library 
# This module defines
#  GSL_LIBRARIES, the libraries needed to use GSL.
#  GSL_INCLUDE, the includes needed to use GSL.
#  GSL_FOUND, If false, do not try to use GSL.
# also defined, but not for general use are
#  GSL_LIBRARY, where to find the GSL library.

SET(GSL_NAMES ${GSL_NAMES} gsl)
FIND_LIBRARY(GSL_LIBRARY
  NAMES ${GSL_NAMES}
  PATHS /usr/lib64/ /usr/lib/ /usr/local/lib64 /usr/local/lib
  )

SET(GSLCBLAS_NAMES ${GSLCBLAS_NAMES} gslcblas)
FIND_LIBRARY(GSLCBLAS_LIBRARY
  NAMES ${GSLCBLAS_NAMES}
  PATHS /usr/lib64/ /usr/lib/ /usr/local/lib64 /usr/local/lib
  )

FIND_PATH(GSL_INCLUDE
  NAMES "gsl/gsl_version.h"
  PATH_SUFFIXES include
  )

IF (GSL_LIBRARY AND GSL_INCLUDE AND GSLCBLAS_LIBRARY)
  SET(GSL_LIBRARIES "${GSL_LIBRARY};${GSLCBLAS_LIBRARY}")
  SET(GSL_FOUND "YES")
ELSE ()
  SET(GSL_FOUND "NO")
ENDIF ()


IF (GSL_FOUND)
   IF (NOT GSL_FIND_QUIETLY)
      MESSAGE(STATUS "Found GSL library: ${GSL_LIBRARIES}")
   ENDIF (NOT GSL_FIND_QUIETLY)
ELSE (GSL_FOUND)
   IF (GSL_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find a GSL library")
   ENDIF (GSL_FIND_REQUIRED)
ENDIF (GSL_FOUND)

# Deprecated declarations.
MARK_AS_ADVANCED(
  GSL_LIBRARY
  )
