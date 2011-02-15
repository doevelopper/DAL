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

# - Check for the presence of BOOST
#
# The following variables are set when BOOST is found:
#  BOOST_FOUND      = Set to true, if all components of BOOST
#                         have been found.
#  BOOST_INCLUDES   = Include path for the header files of BOOST
#  BOOST_LIBRARIES  = Link these to use BOOST
#  BOOST_LFLAGS     = Linker flags (optional)

if (NOT BOOST_FOUND)

  ## Initialization of variables

  if (NOT BOOST_ROOT_DIR)
    set (BOOST_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  endif (NOT BOOST_ROOT_DIR)
  
  set (BOOST_MODULES
    date_time
    filesystem
    graph
    iostreams
    math_c99
    math_c99f
    math_c99l
    math_tr1
    math_tr1f
    math_tr1l
    prg_exec_monitor
    program_options
    python
    random
    regex
    serialization
    signals
    system
    test_exec_monitor
    thread
    unit_test_framework
    wave
    wserialization
    )
  
  ##_____________________________________________________________________________
  ## Check for header files and libraries
  
  set (BOOST_INCLUDES "")
  set (BOOST_LIBRARIES "")

  foreach (_module ${BOOST_MODULES})
    
    ## Convert library name to CMake variable
    string (TOUPPER ${_module} _boost_var)

    ## Search for the include directory
    find_path (BOOST_${_boost_var}_INCLUDES boost/${_module}.hpp
      PATHS ${BOOST_ROOT_DIR} /sw /usr /usr/local /opt/local
      PATH_SUFFIXES include include/boost
      )
    if (BOOST_${_boost_var}_INCLUDES)
      list (APPEND BOOST_INCLUDES ${BOOST_${_boost_var}_INCLUDES})
    endif (BOOST_${_boost_var}_INCLUDES)
    
    ## Search for the library
    find_library (BOOST_${_boost_var}_LIBRARY boost_${_module}
      PATHS ${BOOST_ROOT_DIR} /sw /usr /usr/local /opt/local
      PATH_SUFFIXES lib
      )
    if (BOOST_${_boost_var}_LIBRARY)
      list (APPEND BOOST_LIBRARIES ${BOOST_${_boost_var}_LIBRARY})
    endif (BOOST_${_boost_var}_LIBRARY)
    
  endforeach (_module)

  ## Clean up the list of include directories
  list (REMOVE_DUPLICATES BOOST_INCLUDES)
  
  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (BOOST_INCLUDES AND BOOST_LIBRARIES)
    set (BOOST_FOUND TRUE)
  else (BOOST_INCLUDES AND BOOST_LIBRARIES)
    set (BOOST_FOUND FALSE)
    if (NOT BOOST_FIND_QUIETLY)
      if (NOT BOOST_INCLUDES)
	message (STATUS "Unable to find BOOST header files!")
      endif (NOT BOOST_INCLUDES)
      if (NOT BOOST_LIBRARIES)
	message (STATUS "Unable to find BOOST library files!")
      endif (NOT BOOST_LIBRARIES)
    endif (NOT BOOST_FIND_QUIETLY)
  endif (BOOST_INCLUDES AND BOOST_LIBRARIES)
  
  if (BOOST_FOUND)
    if (NOT BOOST_FIND_QUIETLY)
      message (STATUS "Found components for BOOST")
      message (STATUS "BOOST_INCLUDES  = ${BOOST_INCLUDES}")
      message (STATUS "BOOST_LIBRARIES = ${BOOST_LIBRARIES}")
    endif (NOT BOOST_FIND_QUIETLY)
  else (BOOST_FOUND)
    if (BOOST_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find BOOST!")
    endif (BOOST_FIND_REQUIRED)
  endif (BOOST_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    BOOST_INCLUDES
    BOOST_LIBRARIES
    )
  
endif (NOT BOOST_FOUND)