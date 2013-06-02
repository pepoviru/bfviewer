# - Find a WFDB library 
# This module defines
#  WFDB_LIBRARIES, the libraries needed to use WFDB.
#  WFDB_INCLUDE, the includes needed to use WFDB.
#  WFDB_FOUND, If false, do not try to use WFDB.
# also defined, but not for general use are
#  WFDB_LIBRARY, where to find the WFDB library.

SET(WFDB_NAMES ${WFDB_NAMES} wfdb)
FIND_LIBRARY(WFDB_LIBRARY
  NAMES ${WFDB_NAMES}
  PATHS /usr/lib64/ /usr/lib/ /usr/local/lib64 /usr/local/lib $ENV{WFDBROOT}/lib
  )

FIND_PATH(WFDB_INCLUDE
  NAMES "wfdb/wfdb.h"
  HINTS $ENV{WFDBROOT}
  PATH_SUFFIXES include
  )

IF (WFDB_LIBRARY AND WFDB_INCLUDE)
  SET(WFDB_LIBRARIES ${WFDB_LIBRARY})
  SET(WFDB_FOUND "YES")
ELSE (WFDB_LIBRARY AND WFDB_INCLUDE)
  SET(WFDB_FOUND "NO")
ENDIF ()


IF (WFDB_FOUND)
   IF (NOT WFDB_FIND_QUIETLY)
      MESSAGE(STATUS "Found WFDB library: ${WFDB_LIBRARIES}")
   ENDIF (NOT WFDB_FIND_QUIETLY)
ELSE (WFDB_FOUND)
   IF (WFDB_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find a WFDB library")
   ENDIF (WFDB_FIND_REQUIRED)
ENDIF (WFDB_FOUND)

# Deprecated declarations.
MARK_AS_ADVANCED(
  WFDB_LIBRARY
  )
