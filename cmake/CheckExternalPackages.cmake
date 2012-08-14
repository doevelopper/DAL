
## ==============================================================================
##
##  System inspection
##
## ==============================================================================

##____________________________________________________________________
## Locations to search for required external components

set (DAL_FIND_PATHS
  /usr
  /usr/local
  /opt
  /opt/local
  /sw
  ${CMAKE_INSTALL_PREFIX}
)

## Path used for searching by 'find_file()' and 'find_path()'
set (CMAKE_INCLUDE_PATH ${DAL_FIND_PATHS} )
## Path used for searching by 'find_library()'.
set (CMAKE_LIBRARY_PATH ${DAL_FIND_PATHS} )
## Path used for searching by 'find_program()'.
set (CMAKE_PROGRAM_PATH ${DAL_FIND_PATHS} )

##____________________________________________________________________
##                                          System libraries and tools

foreach (_syslib dl m pthread util)
  
  message (STATUS "Checking for ${_syslib} library ...")

  string (TOUPPER ${_syslib} _syslibVar)

  find_library (${_syslibVar}_LIBRARY
    NAMES ${_syslib}
    PATH_SUFFIXES lib
    )
  
  if (${_syslibVar}_LIBRARY)
    message (STATUS "Checking for ${_syslib} library - Success")
  else (${_syslibVar}_LIBRARY)
    message (STATUS "Checking for ${_syslib} library - FAIL")
  endif (${_syslibVar}_LIBRARY)
  
endforeach (_syslib)

##____________________________________________________________________
##                                                  CMake find modules

## Adjust the module path 
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

## List of modules to be loaded in
set (_cmakeModules
  Boost_DAL
  Casacore
  CFITSIO_DAL
  Doxygen
  Git
  GSL_DAL
  HDF5_DAL
  LAPACK_DAL
  LATEX_DAL
  LOFAR
  MPI
  NumPy
  OpenMP
  WCSLIB_DAL
  )

if (DAL_WITH_MYSQL)
  list (APPEND _cmakeModules MySQL)
endif (DAL_WITH_MYSQL)

foreach (_dalcmake ${_cmakeModules} )

  message (STATUS "Checking for package ${_dalcmake}")

  ## Generate uppercase version of package name
  string (TOUPPER ${_dalcmake} _dalcmake_var)
  ## Strip "_DAL" from module name for usage as internal variable
  string(REPLACE "_DAL" "" _cmakeModuleVariable ${_dalcmake_var})
  ## Initilize CMake varible indicating wether or not package was found
  set (${_dalcmake_var}_FOUND FALSE)
  ## Enable/disable verbosity mode
  if (DAL_VERBOSE_CONFIGURE)
    set (${_cmakeModuleVariable}_FIND_QUIETLY NO )
    set (${_dalcmake}_FIND_QUIETLY            NO )
  else (DAL_VERBOSE_CONFIGURE)
    set (${_cmakeModuleVariable}_FIND_QUIETLY YES )
    set (${_dalcmake}_FIND_QUIETLY            YES )
  endif (DAL_VERBOSE_CONFIGURE)
  ## Include CMake find script
  include (Find${_dalcmake})
  
  if (${_cmakeModuleVariable}_FOUND OR HAVE_${_cmakeModuleVariable})

    ## variable mapping
    set (HAVE_${_cmakeModuleVariable}      TRUE )
    set (${_cmakeModuleVariable}_FOUND     TRUE )
    set (DAL_WITH_${_cmakeModuleVariable}  TRUE )
    
    ## include directories
    if (${_cmakeModuleVariable}_INCLUDES)
      include_directories (${${_cmakeModuleVariable}_INCLUDES})
    endif (${_cmakeModuleVariable}_INCLUDES)
    
    if (${_cmakeModuleVariable}_INCLUDE_PATH)
      include_directories (${${_cmakeModuleVariable}_INCLUDE_PATH})
    endif (${_cmakeModuleVariable}_INCLUDE_PATH)
    
    message (STATUS "Checking for package ${_dalcmake} - Success")
    
  else (${_cmakeModuleVariable}_FOUND OR HAVE_${_cmakeModuleVariable})
  
    message (STATUS "Checking for package ${_dalcmake} - FAIL")

  endif (${_cmakeModuleVariable}_FOUND OR HAVE_${_cmakeModuleVariable})
  
endforeach (_dalcmake)

##____________________________________________________________________
##                                                          Type sizes

if (SIZEOF_LONG_LONG)
  add_definitions (-DHAVE_LONG_LONG)
endif (SIZEOF_LONG_LONG)

if (CMAKE_SIZEOF_VOID_P)
  if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set (WORDSIZE_IS_64 TRUE)
  else (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set (WORDSIZE_IS_64 FALSE)
  endif (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
endif (CMAKE_SIZEOF_VOID_P)

## ==============================================================================
##
##  Build options: evaluation of the system inspection
##
## ==============================================================================

if (NOT DOCUMENTATION_ONLY)
  
  if (NOT HDF5_FOUND)
    message (SEND_ERROR "[DAL] Missing HDF5 library installation!")
  endif (NOT HDF5_FOUND)
  
  if (CASACORE_FOUND)
    if (NOT WCSLIB_FOUND)
      message (SEND_ERROR "[DAL] Missing WCSLIB library installation!")
    endif (NOT WCSLIB_FOUND)
  endif (CASACORE_FOUND)
  
endif (NOT DOCUMENTATION_ONLY)

##____________________________________________________________________
##                                                     Python bindings

## Basic tests for the required external libraries

if (NOT BOOST_FOUND)
  set (DAL_BINDINGS_PYTHON FALSE)
  message (STATUS "[DAL] Unable to generate Python bindings; missing Boost++!")
endif (NOT BOOST_FOUND)

if (NOT PYTHON_FOUND)
  set (DAL_BINDINGS_PYTHON FALSE)
  message (STATUS "[DAL] Unable to generate Python bindings; missing Python!")
endif (NOT PYTHON_FOUND)

if (NOT NUMPY_FOUND)
  set (DAL_BINDINGS_PYTHON FALSE)
  message (STATUS "[DAL] Unable to generate Python bindings; missing NumPy!")
endif (NOT NUMPY_FOUND)

##____________________________________________________________________
##                                                       Documentation

if (NOT DOXYGEN_FOUND)
  set (DAL_WITH_DOCUMENTATION FALSE)
  message (STATUS "Skip generating documentation - Doxygen not found!")
endif (NOT DOXYGEN_FOUND)

##____________________________________________________________________
##                                              casacore Measures data

if (CASACORE_DATADIR)
  set (CASADATA ${CASACORE_DATADIR})
endif (CASACORE_DATADIR)

## ==============================================================================
##
##  Configure file
##
##  Any occurrences of #cmakedefine VAR will be replaced with either #define VAR
##  or /* #undef VAR */ depending on the setting of VAR in CMake. Any occurrences
##  of #cmakedefine01 VAR will be replaced with either #define VAR 1 or
##  #define VAR 0 depending on whether VAR evaluates to TRUE or FALSE in CMake
##
## ==============================================================================

## Generate configure file

configure_file (
  ${DAL_SOURCE_DIR}/cmake/dal_config.h.in
  ${DAL_BINARY_DIR}/dal_config.h
  )

## Installation instruction

install (
  FILES ${DAL_BINARY_DIR}/dal_config.h
  DESTINATION ${DAL_INSTALL_INCLUDEDIR}
  )
