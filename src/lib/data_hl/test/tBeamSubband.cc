/***************************************************************************
 *   Copyright (C) 2006                                                    *
 *   Lars B"ahren (bahren@astron.nl)                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/*!
  \file tBeamSubband.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the DAL::BeamSubband class

  \author Lars B&auml;hren

  \date 2008/09/28
*/

#include <core/dalDataset.h>
#include <data_hl/BeamSubband.h>

using DAL::dalDataset;
using DAL::dalGroup;
using DAL::dalTable;
using DAL::BeamSubband;

// ------------------------------------------------------------------------------

/*!
  \brief Test working with tables through HDF5 library and DAL

  \param filename  -- Name of the input data file
  \param groupName -- Name of the beam group to open and work with
  \param tableName -- Name of the sub-band table to open and work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_tables (std::string const &filename,
                 std::string const &groupName,
                 std::string const &tableName)
{
  std::cout << "\n[tBeamSubband::test_tables]\n" << std::endl;

  int nofFailedTests (0);
  dalDataset *dataset;
  dalGroup * group;
  dalTable * table;

  std::cout << "[1] Open file into dalDataset ..." << std::endl;
  try
    {
      dataset = new dalDataset(filename.c_str(),"HDF5");
      dataset->summary();
    }
  catch (std::string message)
    {
      std::cerr << message << std::endl;
      nofFailedTests++;
    }

  std::cout << "[2] Open beam group from dalDataset ..." << std::endl;
  try
    {
      group = dataset->openGroup(groupName);
      //
      group->summary();
    }
  catch (std::string message)
    {
      std::cerr << message << std::endl;
      nofFailedTests++;
    }

  std::cout << "[3] Open sub-band table from dalDataset ..." << std::endl;
  try
    {
      table = dataset->openTable (tableName,groupName);
      //
      table->summary();
    }
  catch (std::string message)
    {
      std::cerr << message << std::endl;
      nofFailedTests++;
    }

  return nofFailedTests;
}

// ------------------------------------------------------------------------------

/*!
  \brief Test the constructors for an object of type BeamSubband

  \param filename  -- Name of the input data file
  \param groupName -- Name of the beam group to open and work with
  \param tableName -- Name of the sub-band table to open and work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_construction (std::string const &filename,
                       std::string const &groupName,
                       std::string const &tableName)
{
  std::cout << "\n[tBeamSubband::test_construction]\n" << std::endl;

  int nofFailedTests (0);

  std::cout << "[1] Testing default constructor ..." << std::endl;
  try
    {
      BeamSubband subband;
      //
      subband.summary();
    }
  catch (std::string message)
    {
      std::cerr << message << std::endl;
      nofFailedTests++;
    }

  std::cout << "[2] Argumented constructor using dalDataset ..." << std::endl;
  try
    {
      dalDataset dataset (filename.c_str(),"HDF5");
      //
      BeamSubband subband (dataset,groupName,tableName);
      //
      subband.summary();
    }
  catch (std::string message)
    {
      std::cerr << message << std::endl;
      nofFailedTests++;
    }

  std::cout << "[3] Argumented constructor using dalTable ..." << std::endl;
  try
    {
      dalDataset *dataset;
      dalTable * table;
      dataset = new dalDataset(filename.c_str(),"HDF5");
      table = dataset->openTable (tableName,groupName);
      //
      BeamSubband subband (table);
      //
      subband.summary();
    }
  catch (std::string message)
    {
      std::cerr << message << std::endl;
      nofFailedTests++;
    }

  return nofFailedTests;
}

// ------------------------------------------------------------------------------

int main (int argc,char *argv[])
{
  int nofFailedTests (0);
  std::string filename;
  std::string groupName ("beam000");
  std::string tableName ("SB000");

  /* Check command line parameters */

  if (argc > 1)
    {
      filename = std::string(argv[1]);
    }
  else
    {
      std::cout << "[tBeamSubband] Missing name of input test file." << std::endl;
      return(DAL::FAIL);
    }

  nofFailedTests += test_tables (filename,groupName,tableName);

  if (nofFailedTests == 0)
    {
      nofFailedTests += test_construction (filename,groupName,tableName);
    }

  return nofFailedTests;
}
