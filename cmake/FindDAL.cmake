# +-----------------------------------------------------------------------------+
# | $Id::                                                                     $ |
# +-----------------------------------------------------------------------------+
# |   Copyright (C) 2007                                                        |
# |   Lars B"ahren (bahren@astron.nl)                                           |
# |                                                                             |
# |   This program is free software; you can redistribute it and/or modify      |
# |   it under the terms of the GNU General Public License as published by      |
# |   the Free Software Foundation; either version 2 of the License, or         |
# |   (at your option) any later version.                                       |
# |                                                                             |
# |   This program is distributed in the hope that it will be useful,           |
# |   but WITHOUT ANY WARRANTY; without even the implied warranty of            |
# |   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             |
# |   GNU General Public License for more details.                              |
# |                                                                             |
# |   You should have received a copy of the GNU General Public License         |
# |   along with this program; if not, write to the                             |
# |   Free Software Foundation, Inc.,                                           |
# |   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 |
# +-----------------------------------------------------------------------------+

# - Check for the presence of DAL
#
# The following variables are set when DAL is found:
#  DAL_FOUND      = Set to true, if all components of DAL have been found.
#  DAL_INCLUDES   = Include path for the header files of DAL
#  DAL_LIBRARIES  = Link these to use DAL
#  DAL_LFLAGS     = Linker flags (optional)

if (NOT DAL_FOUND)
    
  if (NOT DAL_ROOT_DIR)
    set (DAL_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  endif (NOT DAL_ROOT_DIR)
  
  ##_____________________________________________________________________________
  ## Check for the header files
  
  find_path (DAL_INCLUDES dal_config.h dalCommon.h
    HINTS ${DAL_ROOT_DIR}
    PATHS /sw /usr /usr/local /opt/local
    PATH_SUFFIXES include include/dal
    )
  
  ##_____________________________________________________________________________
  ## Check for the library
  
  find_library (DAL_LIBRARIES dal
    HINTS ${DAL_ROOT_DIR}
    PATHS /sw /usr /usr/local /opt/local
    PATH_SUFFIXES lib
    )
  
  ##_____________________________________________________________________________
  ## Check for the executable
  
#  find_program (DAL_EXECUTABLE <package name>
#    HINTS ${DAL_ROOT_DIR}
#    PATHS /sw /usr /usr/local /opt/local
#    PATH_SUFFIXES bin
#    )
  
  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (DAL_INCLUDES AND DAL_LIBRARIES)
    set (DAL_FOUND TRUE)
  else (DAL_INCLUDES AND DAL_LIBRARIES)
    set (DAL_FOUND FALSE)
    if (NOT DAL_FIND_QUIETLY)
      if (NOT DAL_INCLUDES)
	message (STATUS "Unable to find DAL header files!")
      endif (NOT DAL_INCLUDES)
      if (NOT DAL_LIBRARIES)
	message (STATUS "Unable to find DAL library files!")
      endif (NOT DAL_LIBRARIES)
    endif (NOT DAL_FIND_QUIETLY)
  endif (DAL_INCLUDES AND DAL_LIBRARIES)
  
  if (DAL_FOUND)
    if (NOT DAL_FIND_QUIETLY)
      message (STATUS "Found components for DAL")
      message (STATUS "DAL_ROOT_DIR  = ${DAL_ROOT_DIR}")
      message (STATUS "DAL_INCLUDES  = ${DAL_INCLUDES}")
      message (STATUS "DAL_LIBRARIES = ${DAL_LIBRARIES}")
    endif (NOT DAL_FIND_QUIETLY)
  else (DAL_FOUND)
    if (DAL_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find DAL!")
    endif (DAL_FIND_REQUIRED)
  endif (DAL_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    DAL_INCLUDES
    DAL_LIBRARIES
    )
  
endif (NOT DAL_FOUND)
