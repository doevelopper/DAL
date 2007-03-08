/***************************************************************************
 *   Copyright (C) 2007 by Joseph Masters                                  *
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
/**
 * \file dal.cpp
 * \author Joseph Masters
 * \date 06-Feb-07
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALDATASET_H
#include "dalDataset.h"
#endif

#ifndef DALGROUP_H
#include "dalGroup.h"
#endif

/*! doxygen comment in dal.cpp */
int main(int argc, char *argv[])
{

  // parameter check
  if ( argc < 2 )
  {
     cout << endl << "Too few parameters..." << endl << endl;
     cout << "The first parameter is the dataset name." << endl;
     //cout << "The second parameter is the filetype. (optional)" << endl;
     cout << endl;
     return FAIL;
  }

  dalDataset * dataset;
  if ( NULL == argv[2] )
	  dataset = new dalDataset( argv[1] );
  else
	  dataset = new dalDataset( argv[1], argv[2] );

  //
  /////////////////////////////////////////
  // create the "Station" group
  /////////////////////////////////////////
  //
  dalGroup * stationGroup = dataset->createGroup( "Station" );

  string telescope = "LOFAR";
  string observer = "Iba User";
  string project = "Transients";
  string observation_id = "1287";
  string observation_mode = "Standard";
  string trigger_type = "Unknown";
  double trigger_offset[1] = { 0 };
  int triggered_antennas[1] = { 0 };
  double beam_direction[2] = { 0, 0 };

  // Add attributes to "Station" group
  stationGroup->setAttribute_string("TELESCOPE", telescope );
  stationGroup->setAttribute_string("OBSERVER", observer );
  stationGroup->setAttribute_string("PROJECT", project );
  stationGroup->setAttribute_string("OBSERVATION_ID", observation_id );
  stationGroup->setAttribute_string("OBSERVATION_MODE", observation_mode );
  stationGroup->setAttribute_string("TRIGGER_TYPE", trigger_type );
  stationGroup->setAttribute_double("TRIGGER_OFFSET", trigger_offset );
  stationGroup->setAttribute_int("TRIGGERED_ANTENNAS", triggered_antennas );
  stationGroup->setAttribute_double("BEAM_DIRECTION", beam_direction, 2 );

  //
  /////////////////////////////////////////
  // create ANTENNA table
  /////////////////////////////////////////
  //
  dalTable * AntennaTable = dataset->createTable( "ANTENNA", "Station" );

  // add attributes to ANTENNA table
  unsigned int station_id[1] = { 0 };
  double sample_freq[1] = { 0 };
  unsigned int data_length[1] = { 96 };

  AntennaTable->setAttribute_uint("STATION_ID", station_id );
  AntennaTable->setAttribute_double("SAMPLE_FREQ", sample_freq );
  AntennaTable->setAttribute_uint("DATA_LENGTH", data_length );

  // add columns to ANTENNA table
  AntennaTable->addColumn( "RSP_ID", dal_UINT );  // simple column
  AntennaTable->addColumn( "RCU_ID", dal_UINT );  // simple column
  AntennaTable->addColumn( "TIME", dal_UINT );  // simple column
  AntennaTable->addColumn( "SAMP_NR", dal_UINT );  // simple column
  AntennaTable->addColumn( "SAMP_FRAME", dal_UINT );  // simple column
  AntennaTable->addColumn( "DATA", dal_SHORT);//, data_length[0] );
  AntennaTable->addColumn( "FEED", dal_STRING );
  AntennaTable->addColumn( "ANT_POS", dal_DOUBLE, 3 );
  AntennaTable->addColumn( "ANT_ORIENT", dal_DOUBLE, 3 );

  /*
    The AntennaStruct is ~88 bytes.  I use a buffer of 1,000,000
    so that the writebuffer will be (88 * 1e6) or 88mb.
  */
  const long BufferSIZE = 1000000;
  typedef struct AntennaStruct {
	unsigned int rsp_id;
	unsigned int rcu_id;
	unsigned int time;
	unsigned int sample_nr;
	unsigned int samples_per_frame;
	short data;
	char feed[16];
	double ant_position[ 3 ];
	double ant_orientation[ 3 ];
  } AntennaStruct;

  typedef struct writebuffer {
	AntennaStruct antenna[ BufferSIZE ];
  } writebuffer;

  writebuffer * wb;
  long wbsize = sizeof(writebuffer)/1000000;  // in megabytes
  //cout << "size of write buffer: " << wbsize << " mb";
  if (( wb = (writebuffer *) malloc( sizeof(writebuffer) ) ) == NULL) {
    cout << "ERROR ALLOCATING writebuffer of size " << wbsize << " mb." << endl;
    exit(8);
  }

  const int LOOPMAX = 13;
  //cout << " * " << LOOPMAX << " = " << (wbsize * LOOPMAX) << " mb" << endl;

  // Fill ANTENNA table with data
  for ( int uu=0 ; uu < LOOPMAX; uu++)
  {
	for (long row=0; row<BufferSIZE; row++) {
		wb->antenna[row].rsp_id = (uu*BufferSIZE)+row;
		wb->antenna[row].rcu_id = 1;
		wb->antenna[row].time = 2;
		wb->antenna[row].sample_nr = 3;
		wb->antenna[row].samples_per_frame = 4;
		wb->antenna[row].data = 5;
		strcpy(wb->antenna[row].feed,"hello");
		wb->antenna[row].ant_position[0] = 6;
		wb->antenna[row].ant_position[1] = 7;
		wb->antenna[row].ant_position[2] = 8;
		wb->antenna[row].ant_orientation[0] = 9;
		wb->antenna[row].ant_orientation[1] = 10;
		wb->antenna[row].ant_orientation[2] = 11;
	}
	AntennaTable->appendRows( wb->antenna, BufferSIZE );
  }
  free(wb);  // free the write buffer (~88 mb)

  /////////////////////////////////////////
  // create CALIBRATION table
  /////////////////////////////////////////
  //
  dalTable * CalibrationTable = dataset->createTable( "CALIBRATION", "Station" );

  // add attributes to CALIBRATION table

  // add columns to CALIBRATION table
  CalibrationTable->addColumn( "ADC2VOLT", dal_DOUBLE );  // simple column
  CalibrationTable->addColumn( "GAIN_CURVE", dal_COMPLEX );
  CalibrationTable->addColumn( "GAIN_FREQS", dal_DOUBLE );
  CalibrationTable->addColumn( "BEAM_SHAPE", dal_COMPLEX );
  CalibrationTable->addColumn( "BEAM_DIRS", dal_DOUBLE );
  CalibrationTable->addColumn( "BEAM_FREQS", dal_DOUBLE );
  CalibrationTable->addColumn( "NOISE_CURV", dal_COMPLEX );
  CalibrationTable->addColumn( "NOISE_FREQ", dal_DOUBLE );

  // Fill CALIBRATION table with data
  const long CALBufferSIZE = 10;
  typedef struct CalStruct {
  		double adc2voltage;
  		dalcomplex gain_curve;
  		double gain_frequencies;
  		dalcomplex beam_shape;
  		double beam_directions;
  		double beam_frequencies;
  		dalcomplex noise_curve;
  		double noise_frequencies;
  } CalStruct;

  CalStruct calibration[CALBufferSIZE];
  const int calLOOPMAX = 1;
  for ( int uu=0 ; uu < calLOOPMAX; uu++)
  {
	for (long row=0; row<CALBufferSIZE; row++) {
		calibration[row].adc2voltage = 0;
		calibration[row].gain_curve.r = 1;
		calibration[row].gain_curve.i = 2;
		calibration[row].gain_frequencies = 3;
		calibration[row].beam_shape.r = 4;
		calibration[row].beam_shape.i = 5;
		calibration[row].beam_directions = 6;
		calibration[row].beam_frequencies = 7;
		calibration[row].noise_curve.r = 8;
		calibration[row].noise_curve.i = 9;
		calibration[row].noise_frequencies = 10;
	}
	CalibrationTable->appendRows( calibration, CALBufferSIZE );
  }

  delete AntennaTable;
  delete CalibrationTable;

  delete stationGroup;
  delete dataset;

  cout << "SUCCESS" << endl;
  return SUCCESS;
}