# - Find a QWT library 
# This module defines
#  QWT_LIBRARIES, the libraries needed to use QWT.
#  QWT_INCLUDE, the includes needed to use QWT.
#  QWT_FOUND, If false, do not try to use QWT.
# also defined, but not for general use are
#  QWT_LIBRARY, where to find the QWT library.

SET(QWT_NAMES ${QWT_NAMES} qwt)
FIND_LIBRARY(QWT_LIBRARY
  NAMES ${QWT_NAMES}
  PATHS /usr/lib64/ /usr/lib/ /usr/local/lib64 /usr/local/lib
  )

FIND_PATH(QWT_INCLUDE
  NAMES "qwt/qwt.h"
  PATH_SUFFIXES include
  )

IF (QWT_LIBRARY AND QWT_INCLUDE)
  SET(QWT_LIBRARIES ${QWT_LIBRARY})
  SET(QWT_FOUND "YES")
ELSE (QWT_LIBRARY AND QWT_INCLUDE)
  SET(QWT_FOUND "NO")
ENDIF ()


IF (QWT_FOUND)
   IF (NOT QWT_FIND_QUIETLY)
      MESSAGE(STATUS "Found QWT library: ${QWT_LIBRARIES}")
   ENDIF (NOT QWT_FIND_QUIETLY)
ELSE (QWT_FOUND)
   IF (QWT_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find a QWT library")
   ENDIF (QWT_FIND_REQUIRED)
ENDIF (QWT_FOUND)

# Deprecated declarations.
MARK_AS_ADVANCED(
  QWT_LIBRARY
  )
