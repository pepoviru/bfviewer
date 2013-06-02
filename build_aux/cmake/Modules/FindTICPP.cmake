# - Find a TICPP library 
# This module defines
#  TICPP_LIBRARIES, the libraries needed to use TICPP.
#  TICPP_INCLUDE, the includes needed to use TICPP.
#  TICPP_FOUND, If false, do not try to use TICPP.
# also defined, but not for general use are
#  TICPP_LIBRARY, where to find the TICPP library.

SET(TICPP_NAMES ${TICPP_NAMES} ticpp)
FIND_LIBRARY(TICPP_LIBRARY
  NAMES ${TICPP_NAMES}
  PATHS /usr/lib64/ /usr/lib/ /usr/local/lib64 /usr/local/lib
  )

FIND_PATH(TICPP_INCLUDE
  NAMES "ticpp/ticpp.h"
  PATH_SUFFIXES include
  )

IF (TICPP_LIBRARY AND TICPP_INCLUDE)
  SET(TICPP_LIBRARIES ${TICPP_LIBRARY})
  SET(TICPP_FOUND "YES")
ELSE (TICPP_LIBRARY AND TICPP_INCLUDE)
  SET(TICPP_FOUND "NO")
ENDIF ()


IF (TICPP_FOUND)
   IF (NOT TICPP_FIND_QUIETLY)
      MESSAGE(STATUS "Found TICPP library: ${TICPP_LIBRARIES}")
   ENDIF (NOT TICPP_FIND_QUIETLY)
ELSE (TICPP_FOUND)
   IF (TICPP_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find a TICPP library")
   ENDIF (TICPP_FIND_REQUIRED)
ENDIF (TICPP_FOUND)

# Deprecated declarations.
MARK_AS_ADVANCED(
  TICPP_LIBRARY
  )
