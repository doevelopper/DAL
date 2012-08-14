
# Data Access Library (DAL) #

Contents:

  1. Introduction
  2. Getting the source code
  3. Organization of the source code
  4. External dependencies
  5. Configuration, build and installation
  6. Contributing changes
  7. Further reading

## 1. Introduction ##

The size and complexity of astronomical data are growing at relentless rates.
This increase is especially apparent in the radio community as evidenced by the 
data challenges faced by many of the SKA pathfinders and other major radio
telescopes such as LOFAR, EVLA, ALMA, ASKAP, MeerKAT, MWA, LWA and eMERLIN.
Enormous data rates are also becoming a challenge for large optical projects
that are currently ramping up including Pan-Starrs and LSST. As progress towards
meeting these challenges, ASTRON and the LOFAR project are currently exploring
the use of the Hierarchical Data Format, version 5 (HDF5) for LOFAR radio data
encapsulation.

The Data Access Library (DAL) is a library that abstracts the underlying file
format from the user when working with radio astronomy data. Underlying formats
may include HDF5, AIPS++/CASA tables, FITS or specific raw formats. This library
is available to both the C/C++ developer and the Python user via the "pydal"
module.


## 2. Getting the source code ##

As of Feb 15, 2001 the DAL can be cloned directly from the master repository
hosted on Github (http://github.com):
     
    git clone https://github.com/nextgen-astrodata/DAL.git DAL

Once your checkout from the Git repository is complete, follow the
instructions as described in the section "Configuration and build" below.


## 3. Organization of the source code ##

The components of the DAL is organized into the following directory structure:

    DAL
    |-- build
    |-- cmake
    |-- data                     ..   Reference data sets used for rest programs.
    |-- doc                      ..   Documentation.
    |    `-- figures
    `-- src
          |-- apps               ..   Application programs build on top of the library.
          |    `-- bf2h5
          |-- lib
          |    |-- core          ..   Core library classes
          |    |    `-- test
          |    |-- coordinates   ..   Representation of world coordinates
          |    |    `-- test
          |    |-- data_common   ..   Common functionality for datasets
          |    |    `-- test
          |    `-- data_hl       ..   High-level interfaces to datasets
          |          `-- test
          |-- bindings
          |   `-- python         ..   Bindings to the Python scripting language
          |       `-- test
          `-- test
                |-- hdf5
                |-- python
                `-- swig


## 4. External dependencies ##

To build the DAL from the sources you need a number of installed software
packages and utilities:

| Package                                         | Version  | Status/Description          |
|-------------------------------------------------|----------|-----------------------------|
| [CMake](www.cmake.org)                          | >=2.8    | required                    |
| [Doxygen](http://www.stack.nl/~dimitri/doxygen) | >=1.6    | optional, for documentation |
| [HDF5 library](www.hdfgroup.org/HDF5)           | >=1.8.3  | required, for LOFAR data    |
| [WCSLIB](www.atnf.csiro.au/people/mcalabre/WCS) | >=4.2    | optional, for casacore      |
| [CFITSIO](heasarc.gsfc.nasa.gov/fitsio)         | >=3.0    | optional, for FITS support  |
| [casacore](code.google.com/p/casacore)          |          | optional, for MS support    |
| [Boost libraries](http://www.boost.org)         | >=1.40.0 | optional                    |
| [Python](http://www.python.org)                 | >= 2.6   | optional, Python bindings   |


## 5. Configuration, build and installation ##

The DAL uses the CMake (www.cmake.org) Cross-Platform Makefile Generator for 
configuration and control of the build.

Once your checkout from the Git repository is complete you will have a directory
structure on disk as described in section "Organization of the source code"
above.

    cd DAL
    mkdir build
    cd build
    cmake ..
    make

The default installation location is "/opt/dal"; if you want to change this,
you can tell CMake so by using

    cmake -D DAL_INSTALL_PREFIX=<installation prefix> ..

The CMake scripts will check a number of standard locations for the required
3-party packages (such as Boost, HDF5, etc.), but if you are using a custom
version installed at a non-standard location, you might need to aid CMake in
finding all required components:

    cmake -DHDF5_ROOT_DIR=<basedir of HDF5 install> ..

In to install the DAL you will have to run

    make install

from within the build directory. Depending on whether you are installing into a
local directory or into a system-directory, appropriate permissions might be
required.

In order to maintain compatibility with the Filesystem Hierarchy Standard (FHS)
the following default installation layout has been defined:

    /opt
    └── dal                            CMAKE_INSTALL_PREFIX   = DAL_INSTALL_PREFIX
        ├── bin                        DAL_INSTALL_BINDIR     = DAL_INSTALL_PREFIX/bin
        ├── include                    DAL_INSTALL_INCLUDEDIR = DAL_INSTALL_PREFIX/include
        │   ├── coordinates
        │   ├── core
        │   ├── data_common
        │   └── data_hl
        ├── lib                        DAL_INSTALL_LIBDIR     = DAL_INSTALL_PREFIX/lib
        └── share
            └── doc
                └── html

However the installation configuration can be tailored depending on your personal
needs:

  1. Default settings.

     If no further command line parameter are provided, the installation will be
     done according to the above described structure.

  2. Custom installation prefix.

     In order to install into an existing standard directory structure -- such
     such as e.g. "/usr/local" -- use

        -D DAL_INSTALL_PREFIX=<prefix>

  3. Installation as part of the LUS.

     In order to simplify installation of the DAL as part of the LOFAR User
     Software distribution, a preset has been defined which can be used through

        -D LUS_INSTALL_PREFIX=<prefix>

     where <prefix>=$LOFARSOFT/release

For further details and additional configuration options, consult the Doxygen-based
documentation.


## 6. Contributing changes ##

Direct write access to the project's code respository is limited to a small core
team, responsible for the guiding the development of the DAL. If you are in more
than simply using the DAL, but actively contributing to it development, the 
advised work-flow is as follows:

  1. Create your own fork from the master repository of the project.

     CAUTION: If you are planning to to actually development work, make sure you
              fork from the 'master' branch, __not__ from 'release'. Following
              common Git policy create a new branch on which to carry out the
              development - this will make separation from other development 
              activities easier and will enable more granularity in the selection
              of new features into the main development branch or the preparation
              of a new release version.

  2. Work on push changes you are working on - be it bug fixes or feature
     enhancements - back to your fork.

  3. When ready to have your changes integrated into the project's development
     master branch, issue a pull request. This will give the core team a chance 
     to have a look at your contribution, before merging it into the master
     branch. If everything is just fine, the pull request will be requested right
     away, otherwise a short conversation might be necessary of how to go about
     the integration of your code contribution.

Suggested reading:

  - http://help.github.com/fork-a-repo (Forking a repository on Github)
  - http://help.github.com/send-pull-requests (sending pull requests on Github)


## 7. Further reading ##

Further detailed information regarding

  - configuration options
  - installation options
  - organization of the code base
  - library modules

can be found in the HTML documentation generated using Doxygen:

    cd build
    cmake ..
    make Documentation

If you are having trouble with unresolved package dependencies, you can configure
for generation of documentation only:

    cmake -DDOCUMENTATION_ONLY=YES ..
    make

Point your web-browser to

    build/doc/html/index.html

