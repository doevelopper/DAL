/***************************************************************************
 *   Copyright (C) 2006 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
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
  \file tBeamGroup.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the DAL::BeamGroup class
  \author Lars B&auml;hren

  \date 2008/09/19

  This test program mainly is going through the functionality of the
  DAL::BeamGroup class, but also - at least in an indirect way - some of
  DAL::BeamSubband.
*/

#include <core/dalCommon.h>
#include <core/dalDataset.h>
#include <data_hl/BeamFormed.h>
#include <data_hl/BeamGroup.h>

using std::cout;
using std::endl;

// ------------------------------------------------------------------------------

/*!
  \brief Test the various constructors for an object of type BeamGroup

  \param filename  -- Name of the input data file
  \param groupName -- Name of the beam group to open and work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_constructors (std::string const &filename,
                       std::string const &groupName)
{
  cout << "\n[tBeamGroup::test_constructors]\n" << endl;

  int nofFailedTests (0);

  cout << "[1] Default constructor..." << endl;
  try
    {
      DAL::BeamGroup group;
      //
      group.summary();
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[2] Argumented constructor..." << endl;
  try
    {
      DAL::dalDataset dataset;
      dataset.open(filename.c_str());
      //
      DAL::BeamGroup group (dataset,groupName);
      //
      group.summary();
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[3] Construction from BeamFormed object ..." << endl;
  try
    {
      cout << "-- Creating BeamFormed object ..." << endl;
      DAL::BeamFormed bf (filename);
      //
      cout << "-- Extracting beam group from dataset ..." << endl;
      int numBeam (0);
      DAL::BeamGroup * group = bf.getBeam (numBeam);
      //
      group->summary();
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  return nofFailedTests;
}

// ------------------------------------------------------------------------------

/*!
  \brief Test extraction of the various attributes

  \param filename  -- Name of the input data file
  \param groupName -- Name of the beam group to open and work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_attributes (std::string const &filename,
                     std::string const &groupName)
{
  cout << "\n[tBeamGroup::test_attributes]\n" << endl;

  int nofFailedTests (0);

  DAL::dalDataset dataset;
  dataset.open(filename.c_str());
  DAL::BeamGroup group (dataset,groupName);

  cout << "[1] Attributes attached to the beam group ..." << endl;
  try
    {
      cout << "-- fileID()      = " << group.fileID()      << endl;
      cout << "-- groupID()     = " << group.groupID()     << endl;
      cout << "-- groupName()   = " << group.groupName()   << endl;
      cout << "-- ra()          = " << group.ra()          << endl;
      cout << "-- dec()         = " << group.dec()         << endl;
      cout << "-- nofSubbands() = " << group.nofSubbands() << endl;
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[2] Attributes attached to the sub-band tables ..." << endl;
  try
    {
      /*
       *  Center frequencies [MHz] of the sub-bands
       */
      std::vector<int> freq = group.center_frequencies();
      cout << "-- Center frequencies = [";
      for (uint n(0); n<freq.size(); n++)
        {
          cout << " " << freq[n];
        }
      cout << " ]" << endl;
      /*
       * Number of rows in the sub-band tables
       */
      std::vector<long> rows = group.nofTableRows();
      cout << "-- nof. table rows    = [";
      for (uint n(0); n<rows.size(); n++)
        {
          cout << " " << rows[n];
        }
      cout << " ]" << endl;
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  return nofFailedTests;
}

// ------------------------------------------------------------------------------

/*!
  \brief Test the various methods for operation on the data

  \param filename  -- Name of the input data file
  \param groupName -- Name of the beam group to open and work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_methods (std::string const &filename,
                  std::string const &groupName)
{
  cout << "\n[tBeamGroup::test_methods]\n" << endl;

  int nofFailedTests (0);
  int start (0);
  int length (20);

  DAL::dalDataset dataset;
  dataset.open(filename.c_str());
  DAL::BeamGroup group (dataset,groupName);
  int nofSubbands (group.nofSubbands());
  std::vector<long> rows = group.nofTableRows();

  cout << "[1] Extract Subband objects from beam-group..." << endl;
  try
    {
      if (nofSubbands > 0)
        {
          DAL::BeamSubband * subband;
          /* Extract the n-th subband from the beam-group */
          for (int n(0); n<nofSubbands; n++)
            {
              subband = group.getSubband (n);
              //
              cout << "  Sub-band = " << n;
              cout << "  ID = " << subband->tableID() << endl;
            }
        }
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[2] Retrieve data via getSubbandData(int,int,&int,vector<complex>) ..."
       << endl;
  try
    {
      /* Arrays for taking up the returned data */
      std::vector<std::complex<short> > dataX (length);
      std::vector<std::complex<short> > dataY (length);
      /* Go through the subbands and retrieve the data */
      for (int n(0); n<nofSubbands; n++)
        {
          /* Extract data values from the table */
          group.getSubbandData_X (n,start,length,dataX);
          group.getSubbandData_Y (n,start,length,dataY);
          /* Print the extracted values */
          cout << "-- Data from subband " << n << " ..." << endl;
          cout << dataX << endl;
          cout << dataY << endl;
        }
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[3] Retrieve data via getSubbandDataXY(...) ..." << endl;
  try
    {
      /* Arrays for taking up the returned data */
      std::vector<std::complex<short> > dataX (length);
      std::vector<std::complex<short> > dataY (length);
      /* Go through the subbands and retrieve the data */
      for (int n(0); n<nofSubbands; n++)
        {
          /* Extract data values from the table */
          group.getSubbandData_XY (n,start,length,dataX,dataY);
          /* Print the extracted values */
          cout << "-- Data from subband " << n << " ..." << endl;
          cout << dataX << endl;
          cout << dataY << endl;
        }
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[4] Test stepping through the data columns ..." << endl;
  try
    {
      int subband (0);
      int nofBlocks (20);
      /* Arrays for taking up the returned data */
      std::vector<std::complex<short> > dataX (length);
      /* Go through the subbands and retrieve the data */
      for (int n(0); n<nofBlocks; n++)
        {
          /* Extract data values from the table */
          group.getSubbandData_X (subband,start,length,dataX);
          /* Print the extracted values */
          cout << dataX << endl;
          /* Increment starting position*/
          start += length;
        }
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[5] Test data retrieval using different blocksizes ..." << endl;
  try
    {
      int subband (0);
      int increment (1024);
      int nofIncrements (20);
      start  = 0;
      length = increment;
      /* Arrays for taking up the returned data */
      std::vector<std::complex<short> > dataX (length);
      /* Go through the subbands and retrieve the data */
      for (int n(0); n<nofIncrements; n++)
        {
          cout << "-- Requesting " << length << " values from X column ..." << endl;
          /* Extract data values from the table */
          group.getSubbandData_X (subband,start,length,dataX);
          /* Increment starting position*/
          length += increment;
        }
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
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

  /* Check command line parameters */

  if (argc > 1)
    {
      filename = std::string(argv[1]);
    }
  else
    {
      cout << "[tBeamGroup] Missing name of input test file." << endl;
      return(DAL::FAIL);
    }

  /* Test the constructors */
  nofFailedTests += test_constructors(filename,groupName);

  if (nofFailedTests == 0)
    {
      nofFailedTests += test_attributes(filename,groupName);
      nofFailedTests += test_methods(filename,groupName);
    }

  return nofFailedTests;
}
