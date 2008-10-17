/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008 by Joseph Masters                                  *
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

#ifndef BFRAW_H
#define BFRAW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

// socket headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PI 3.141592653589793238462643

namespace DAL {
  
  /*!
    \class BFRaw

    \ingroup DAL

    \brief High-level interface between raw beam-formed data and the DAL.

    \author Joseph Masters
    
    High-level interface between raw beam-formed data and the DAL.
  */
  
  class BFRaw
  {
    
    struct FileHeader
    {
      UInt32    magic;        // 0x3F8304EC, also determines endianness
      UInt8     bitsPerSample;
      UInt8     nrPolarizations;
      UInt16    nrBeamlets;   // number of subbands
      UInt32    nrSamplesPerBeamlet; // 155648 (160Mhz) or 196608 (200Mhz)
      char      station[20];
      Float64   sampleRate;       //156250.0 or 195312.5 .. double
      Float64   subbandFrequencies[54];
      Float64   beamDirections[8][2];
      Int16     beamlet2beams[54];
      UInt32    padding;  // padding to circumvent 8-byte alignment
    };

    //! Components of the header of a single block of raw data
    struct BlockHeader
    {
      UInt32      magic; // 0x2913D852
      Int32       coarseDelayApplied[8];
      UInt8       padding[4];  // padding to circumvent 8-byte alignment
      Float64     fineDelayRemainingAtBegin[8];
      Float64     fineDelayRemainingAfterEnd[8];
      Int64       time[8]; // compatible with TimeStamp class.
      UInt32      nrFlagsRanges[8];
      
      struct range
      {
	UInt32    begin; // inclusive
	UInt32    end;   // exclusive
      } flagsRanges[8][16];
      
    };
    
    // dataStruct is 8 bytes
    struct Sample
    {
      complex<int16_t> xx;
      complex<int16_t> yy;
    };
    
    char * DECrad2deg( const float &rad );
    char * RArad2deg( const float &rad );
    
    // declare handle for the input file
    dalTable ** table;
    fstream * rawfile;
    string outputfilename;
    dalDataset dataset;
    FileHeader fileheader;
    BlockHeader blockheader;
    bool bigendian;
    bool first_block;
    int downsample_factor;
    bool doDownsample_p;
    bool DO_FLOAT32_INTENSITY;
    bool DO_CHANNELIZATION;
    bool processBlocks( int16_t );
    Float32 * downsample_to_float32_intensity( Sample *,
					       int32_t,
					       const uint64_t,
					       int32_t );
    Float32 * compute_float32_intensity( Sample * data,
					 int32_t start,
					 const uint64_t arraylength );
  public:

    //! Argumented constructor
    BFRaw( string const& name,
	   bool doIntensity=false,
	   bool doDownsample=false,
	   bool doChannelization=false,
	   int factor=1 );
    
    /* Destruction */
    
    ~BFRaw();

    /* Parameter access */
    
    //! Provide a summary of the object's properties
    inline void summary () { summary(std::cout); }
    //! Provide a summary of the object's properties
    void summary (std::ostream &os);
    //! Get the name of the output file
    inline string outfile () const { return outputfilename; }
    //! Get the number of bits per sample
    inline UInt8 bitsPerSample () const { return fileheader.bitsPerSample; }
    //! Get the number of polarizations
    inline UInt8 nofPolarizations () const { return fileheader.nrPolarizations; }
    //! Get the number of beamlets
    inline UInt16 nofBeamlets () const { return fileheader.nrBeamlets;}
    
    /* Methods */
    
    //! Create HDF5 file from the raw data
    void makeH5OutputFile();
    //! Read the header block of the file with the raw data
    void readRawFileHeader();
    //! Open file with raw output of beamformer
    void openRawFile(char*);
    //! Check for reaching end-of-file
    bool eof();
    //! Processing of the data blocks within the input file
    void processBlocks();

  }; // class BFRaw
  
} // DAL namespace


#endif // BFRAW_H