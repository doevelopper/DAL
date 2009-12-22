/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2007                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#ifndef TBB_STATIONGROUP_H
#define TBB_STATIONGROUP_H

// Standard library header files
#include <iostream>
#include <map>
#include <string>

#ifdef HAVE_CASA
#include <casa/Arrays/ArrayIO.h>
#include <casa/Arrays/Matrix.h>
#include <measures/Measures/MDirection.h>
#endif

#include <CommonInterface.h>
#include <TBB_DipoleDataset.h>

namespace DAL {   // Namespace DAL -- begin

  /*!
    \class TBB_StationGroup

    \ingroup DAL
    \ingroup data_hl

    \brief Container for the data in the StationGroup of LOFAR times-series data

    \author Lars B&auml;hren

    \date 2007/12/10

    \test tTBB_StationGroup.cc

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>Definition of the LOFAR time-series data format
      <li>DAL::TBB_Timeseries
      <li>DAL::TBB_DipoleDataset
      <li>CR::LOFAR_TBB -- Interface between Data Access Library (DAL) and
      CR::DataReader framework
    </ul>

    <h3>Synopsis</h3>

    <ol>
      <li>Structure of the \ref dal_formats_tbb
    </ol>

    <h3>Example(s)</h3>

    <ol>
      <li>Object construction using filename and full name/path of the group
      within the file:
      \code
      std::string filename = "data.h5";
      std::string group    = "Station001";

      TBB_StationGroup group (filename,
                                group);
      \endcode
      <li>Retrieve the list of channels IDs contained within this group:
      \code
      std::string <std::vector> channelID = group.channelID();
      \endcode
      which will return something along the lines of
      \verbatim
      [ 001000000 001000001 .. 001002003 ]
      \endverbatim
      <li>Retrieve a block of samples from the data channels (i.e. dipoles)
      belonging to this station:
      \code
      // TBB_StationGroup constructed somewhere above...

      // set sample at which to start reading
      int start      = 0;
      // set the number of samples to read per data channel (dipole)
      int nofSamples = 1024;

      casa::Matrix<short> data = group.fx (start,
                                           nofSamples);
      \endcode
    </ol>

  */
  class TBB_StationGroup : public CommonInterface {
    
    //! Datasets contained within this group
    std::map<std::string,TBB_DipoleDataset> datasets_p;
    //! Number of triggered antennas at this station
    uint nofTriggeredAntennas_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    TBB_StationGroup ();
    
    //! Argumented constructor
    TBB_StationGroup (hid_t const &location,
		      std::string const &group,
		      bool const &create=true);
    
    //! Argumented constructor
    TBB_StationGroup (hid_t const &groupID);
    
    //! Copy constructor
    TBB_StationGroup (TBB_StationGroup const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~TBB_StationGroup ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    TBB_StationGroup& operator= (TBB_StationGroup const &other);
    
    // === Parameter access =====================================================
    
    
    //! Get the name for this group within the HDF5 file
    std::string group_name (bool const &stripPath=false);
    
    //! Get the number of triggered antennas at this station
    inline uint nofTriggeredAntennas () const {
      return nofTriggeredAntennas_p;
    }
    
#ifdef HAVE_CASA
    //! Get the numerical value of the beam pointing direction.
    casa::Vector<double> beam_direction_value ();
    //! Get the physical units for the beam direction
    casa::Vector<casa::String> beam_direction_unit ();
    //! Get the numerical values of the station position
    casa::Vector<double> station_position_value ();
    //! Get the physical units for the station position
    casa::Vector<casa::String> station_position_unit ();
#else
    //! Get the numerical value of the beam pointing direction.
    std::vector<double> beam_direction_value ();
    //! Get the physical units for the beam direction
    std::vector<std::string> beam_direction_unit ();
    //! Get the numerical values of the station position
    std::vector<double> station_position_value ();
    //! Get the physical units for the station position
    std::vector<std::string> station_position_unit ();
#endif
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, TBB_StationGroup.
    */
    std::string className () const {
      return "TBB_StationGroup";
    }
    
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    
    /*!
      \brief Provide a summary of the internal status
    */
    void summary (std::ostream &os);
    
    // === Methods ==============================================================
    
    //! Open a structure (file, group, dataset, etc.)
    bool open (hid_t const &location);

    //! Open a station group
    bool open (hid_t const &location,
	       std::string const &name,
	       bool const &create=true);
    
    /*!
      \brief Get the number of dipole datasets within this station group
      
      \return nofDipoleDatasets -- The number of dipole datasets contained with
      this station group.
    */
    inline uint nofDipoleDatasets () {
      return datasets_p.size();
    }
    
    //! Get the groupname for a station identified by <tt>index</tt>
    static std::string getName (unsigned int const &index);
    
    // ==========================================================================
    //
    //  Parameter access - dipole dataset
    //
    // ==========================================================================

    //! Get attributes <tt>name</tt> from the embedded dipole datasets
    template <typename T>
      bool getAttributes (std::string const &name,
			  std::vector<T> &result)
      {
	bool status (true);
	
	if (location_p > 0) {
	  uint n(0);
	  T tmp;
	  std::map<std::string,TBB_DipoleDataset>::iterator it;
	  
	  result.resize(datasets_p.size());
	  
	  for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
	    it->second.getAttribute(name,tmp);
	    result[n] = tmp;
	    ++n;
	  }
	} else {
	  status = false;
	}
	return status;
      }
    
    //! Get attributes <tt>name</tt> from the embedded dipole datasets
#ifdef HAVE_CASA
    template <typename T>
      bool getAttributes (std::string const &name,
			  casa::Vector<T> &result)
      {
	bool status (true);
	
	if (location_p > 0) {
	  uint n(0);
	  T tmp;
	  std::map<std::string,TBB_DipoleDataset>::iterator it;
	  
	  result.resize(datasets_p.size());
	  
	  for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
	    it->second.getAttribute(name,tmp);
	    result(n) = tmp;
	    ++n;
	  }
	} else {
	  status = false;
	}
	return status;
      }
    //! Get the sample frequencies as casa::Quantity
    bool sample_frequency (casa::Vector<casa::Quantity> &freq);
    //! Get the sample frequencies as casa::Measure
    bool sample_frequency (casa::Vector<casa::MFrequency> &freq);
    //! Time offset between the individual antennas in units of samples
    casa::Vector<int> sample_offset (uint const &refAntenna=0);
    //! Get the numerical values of the antenna positions within this station.
    casa::Matrix<double> antenna_position_value ();
    //! Get the physical units for the antenna positions within this station.
    casa::Matrix<casa::String> antenna_position_unit ();
    //! Get the positions of the antennas within this station as Measure.
    casa::Vector<casa::MPosition> antenna_position ();
    //! Retrieve the list of channels IDs contained within this group
    casa::Vector<int> channelID ();
    //! Retrieve the list of channels names contained within this group
    casa::Vector<casa::String> channelNames ();
    //! Get identifiers to the datasets within the station group
    casa::Vector<hid_t> datasetIDs ();
#else
    //! Time offset between the individual antennas in units of samples
    std::vector<int> sample_offset (uint const &refAntenna=0);
    //! Retrieve the list of channels IDs contained within this group
    std::vector<int> channelID ();
    //! Retrieve the list of channels names contained within this group
    std::vector<std::string> channelNames ();
    //! Get identifiers to the datasets within the station group
    std::vector<hid_t> datasetIDs ();
#endif
    
    /*!
      \brief Convert individual ID number to joint unique ID
      
      \param station_id -- Identification number of the LOFAR station within the
      complete array.
      \param rsp_id -- Identification number of a Remote Station Processing
      (RSP) board within a given LOFAR station.
      \param rcu_id -- Identification number of a Receiver Unit (RCU) attached to
      a given RSP board.
      
      \return channel_id -- Unique identifier string for an individual dipole
      within the full LOFAR array.
    */
    inline std::string channelID (unsigned int const &station_id,
				  unsigned int const &rsp_id,
				  unsigned int const &rcu_id)
    {
      char uid[10];
      sprintf(uid, "%03d%03d%03d",
	      station_id,rsp_id,rcu_id);
      std::string channel_id (uid);
      return channel_id;
    }
    
    // ============================================================================
    //
    //  Methods using casacore
    //
    // ============================================================================
    
#ifdef HAVE_CASA
    
    //! Get the positions of all antennas in the station
    casa::Vector<casa::MPosition> antennaPositions ();
    
    //! Get the direction of the station beam as casa::Measure
    casa::MDirection beam_direction ();
    
    //! Get the position of the station as a casa::Measure
    casa::MPosition station_position ();
    
    //! Retrieve a block of ADC values for the dipoles in this station
    casa::Matrix<double> fx (int const &start=0,
			     int const &nofSamples=1);
    
    //! Retrieve a block of ADC values for the dipoles in this station
    casa::Matrix<double> fx (int const &start,
			     int const &nofSamples,
			     casa::Vector<casa::String> const &dipoles);
    
    //! Get a casa::Record containing the values of the attributes
    casa::Record attributes2record (bool const &recursive=false);
    
    //! Get a casa::Record containing the values of the attributes
    bool attributes2record (casa::Record &rec,
			    bool const &recursive=false);
#endif
    
  protected:
    
    //! Set up the list of attributes attached to the structure
    void setAttributes ();
    //! Open the structures embedded within the current one
    bool openEmbedded (bool const &create);

  private:
    
    //! Initialize the internal dataspace
    void init ();
    
    //! Unconditional copying
    void copy (TBB_StationGroup const &other);
    
    //! Unconditional deletion
    void destroy(void);
    
  };
  
} // Namespace DAL -- end

#endif /* TBB_STATIONGROUP_H */

