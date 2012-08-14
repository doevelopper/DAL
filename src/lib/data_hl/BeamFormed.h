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

#ifndef BEAMFORMED_H
#define BEAMFORMED_H

#include <data_common/CommonAttributes.h>
#include <data_hl/BeamGroup.h>

namespace DAL {
  
  /*!
    \class BeamFormed
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface between beam-formed data and the DAL

    \author Joseph Masters

    \test tBeamFormed.cpp

    <h3>Synopsis</h3>

    While this class implements the top-level structure of a HDF5 file storing
    beam-formed data, the lower-level structures are accessible through the
    classes BeamGroup and BeamSubband.

    <table border="0">
      <tr>
        <td class="indexkey"></td>
	<td class="indexkey">DAL::BeamFormed</td>
	<td class="indexkey">DAL::BeamGroup</td>
	<td class="indexkey">DAL::BeamSubband</td>
	</tr>
      <tr>
        <td class="indexkey">DAL object</td>
	<td>dalDataset *dataset_p;</td>
	<td>dalGroup *group_p;</td>
	<td>dalTable *table_p;</td>
      </tr>
      <tr>
        <td class="indexkey">HDF5 object IDs</td>
	<td>hid_t H5fileID_p;</td>
	<td>hid_t H5fileID_p;</td>
	<td>hid_t H5fileID_p;</td>
      </tr>
      <tr>
        <td class="indexkey">Subcomponents container</td>
	<td>std::vector<BeamGroup*> beamGroups_p;</td>
	<td>std::vector<BeamSubband*> beamSubbands_p;</td>
	<td>-</td>
      </tr>
    </table>

    <h3>Examples</h3>

    <ol>
      <li>Create an object of type BeamFormed within your class/application:
      \code
      #include <dal/BeamFormed.h>

      DAL::BeamFormed bf (filename);
      \endcode

      <li>Retrieve attributes in the root group of the file:
      \code
      std::string filename      = bf.filename();
      std::string telescope     = bf.telescope();
      std::string observationID = bf.observation_id();
      \endcode
    </ol>
  */

  class BeamFormed {
    
  private:
    
    bool status;
    //! LOFAR common attributes
    CommonAttributes commonAttributes_p;
    //! Filename of the dataset
    std::string filename_p;
    //! HDF5 file handle ID
    hid_t H5fileID_p;
    //! DAL Dataset object to handle the basic I/O
    dalDataset * dataset_p;
    //! Vector of beam groups within the dataset
    std::vector<BeamGroup*> beamGroups_p;
    
  public:

    // === Construction =========================================================
    
    //! Default constructor
    BeamFormed ();

    //! Argumented constructor
    BeamFormed (std::string const &filename);

    //! Argumented constructor
    BeamFormed (CommonAttributes const &commonAttributes);

    //! Destructor
    ~BeamFormed ();

    //___________________________________________________________________________
    //                                                       Access to attributes

    //! Get LOFAR common attributes
    inline CommonAttributes commonAttributes () const {
      return commonAttributes_p;
    }
    //! Set LOFAR common attributes
    bool setCommonAttributes (CommonAttributes const &attributes);
    
    std::vector< std::string > h5get_str_array_attr( std::string attrname,
						     hid_t obj_id );
    std::vector<std::string> beams();
    BeamGroup * getBeam( int beam );
    //! Get the name of the data file
    std::string filename();
    //! Get the name of the telescope
    std::string telescope();
    //! Get the number of stations
    int nofStations();
    //! Get the data type
    std::string datatype();
    //! Get the EM-band
    std::string emband();
    //! Get the source list
    std::vector<std::string> sources();
    //! Get the observation identifier
    std::string observation_id();
    //! Get the project identifier
    std::string proj_id();
    //! RA of the beam pointing direction
    std::string point_ra();
    //! DEC of the beam pointing direction
    std::string point_dec();
    //! Get the name of the observer
    std::string observer();
    //! The observation epoch as Median Julian Day (MJD)
    std::string epoch_mjd();
    //! The observation epoch as data
    std::string epoch_date();
    //! The observation epoch as UTC
    std::string epoch_utc();
    std::string epoch_lst();
    //! Get the diameter of the main beam
    int main_beam_diam();
    //! Get the center frequency
    int center_freq();
    int bandwidth();
    double total_integration_time();
    int breaks();
    //! Get the dispersion measure
    int dispersion_measure();
    int number_of_samples();
    double sampling_time();
    int number_of_beams();
    int sub_beam_diameter();
    int weather_temperature();
    int weather_humidity();
    std::vector<int> station_temperatures();
    //! Get the notes attached to the data set
    std::string notes();
    /*! \brief Provide a summary of the internal status */
    inline void summary () {
      summary (cout, true);
    }
    void summary (std::ostream &os,
		  bool const &listBeams=false);
    
    // --------------------------------------------------------------------------
    // Access to the data stored in the sub-bands
    
    //! Get the X column data for a given subband.
    void getSubbandData_X (int &beam,
			   int &subband,
			   int &start,
			   int &length,
			   std::vector<std::complex<short> > &data);
    //! Get the Y column data for a given subband.
    void getSubbandData_Y (int &beam,
			   int &subband,
			   int &start,
			   int &length,
			   std::vector<std::complex<short> > &data);

    // === Private methods ======================================================
    
  private:

    //! Initialize the object's internal parameters
    bool init ();
    //! Initialize the object's internal parameters
    bool init (std::string const &filename);
    //! Initialize the object's internal parameters
    bool init (CommonAttributes const &commonAttributes);

    //! Get attribute of type string
    std::string getAttribute (std::string const &name);

  }; // BeamFormed class
  
} // namespace DAL

#endif // BEAMFORMED_H
