/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#ifndef BF_STOKESDATASET_H
#define BF_STOKESDATASET_H

// Standard library header files
#include <iostream>
#include <string>

#include <HDF5Dataset.h>
#include <Stokes.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class BF_StokesDataset
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Brief description for class BF_StokesDataset
    
    \author Lars B&auml;hren

    \date 2010/12/05

    \test tBF_StokesDataset.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>\ref dal_icd_003
      <li>DAL::Stokes -- Components of a Stokes coordinate.
      <li>DAL::BF_BeamGroup -- High-level interface to the station beam of a BF
          dataset.
      <li>DAL::BF_SubArrayPointing
    </ul>
    
    <h3>Synopsis</h3>

    Within each Beam group, there are either one or four Stokes Datasets; or the
    Stokes Group contains BF RAW data with two 32-bit complex numbers (containing
    Xreal, Ximg, Yreal and Yimg).  If the data are summed, then there is only one
    Stokes I Group containing all the channel intensities per subbeam. If the
    data are not summed, then there are four Stokes tables -- \f$ (I, Q, U, V) \f$
    or \f$ (XX, XY, YX, YY) \f$ -- one per polarization, containing all the channel
    intensities per subbeam.

    \verbatim
    .                         Dataset
    |-- GROUPTYPE             Attribute           string
    |-- DATATYPE              Attribute           string
    |-- STOKES_COMPONENT      Attribute           string
    |-- NOF_SUBBANDS          Attribute           int
    |-- NOF_CHANNELS          Attribute           int
    \endverbatim

    The datatype of the elements stored within the dataset depends on the Stokes
    component to be recorded:
    
    <table>
      <tr>
        <td class="indexkey">Description</td>
        <td class="indexkey">Stokes</td>
        <td class="indexkey">Datatype</td>
        <td class="indexkey">Specification</td>
        <td class="indexkey">HDF5 datatype</td>
      </tr>
      <tr>
        <td>Complex voltages</td>
        <td>\f$ (X,Y) \f$</td>
        <td>fcomplex</td>
        <td>2 x 32 bit IEEE floating point</td>
	<td>(H5T_NATIVE_FLOAT,H5T_NATIVE_FLOAT)</td>
      </tr>
      <tr>
        <td>Coherent Stokes</td>
        <td>\f$ (I,Q,U,V) \f$</td>
        <td>float</td>
        <td>32 bit IEEE floating point</td>
	<td>H5T_NATIVE_FLOAT</td>
      </tr>
      <tr>
        <td>Incoherent Stokes</td>
        <td>\f$ (I,Q,U,V) \f$</td>
        <td>float</td>
        <td>32 bit IEEE floating point</td>
	<td>H5T_NATIVE_FLOAT</td>
      </tr>
    </table>
    
    <h3>Example(s)</h3>
    
  */  
  class BF_StokesDataset : public HDF5Dataset {

    //! Stokes component stored inside this dataset
    DAL::Stokes itsStokesComponent;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    BF_StokesDataset ();
    
    //! Argumented constructor, opening an existing Stokes dataset
    BF_StokesDataset (hid_t const &location,
		      std::string const &name);

    //! Argumented constructor, creating a new Stokes dataset
    BF_StokesDataset (hid_t const &location,
		      std::string const &name,
		      std::vector<hsize_t> const &shape,
		      DAL::Stokes::Component const &component=DAL::Stokes::I,
		      hid_t const &datatype=H5T_NATIVE_FLOAT);
    
    //! Argumented constructor, creating a new Stokes dataset
    BF_StokesDataset (hid_t const &location,
		      std::vector<hsize_t> const &shape,
		      DAL::Stokes::Component const &component=DAL::Stokes::I);
    
    //! Copy constructor
    BF_StokesDataset (BF_StokesDataset const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~BF_StokesDataset ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    BF_StokesDataset& operator= (BF_StokesDataset const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, BF_StokesDataset.
    */
    inline std::string className () const {
      return "BF_StokesDataset";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Methods ==============================================================
    
    //! Get Stokes component stored within this dataset
    inline DAL::Stokes stokesComponent () const {
      return itsStokesComponent;
    }
    
    //! Get name of Stokes component stored within this dataset
    inline std::string stokesComponentName () const {
      return itsStokesComponent.name();
    }

    //! Get type of Stokes component stored within this dataset
    inline DAL::Stokes::Component stokesComponentType () const {
      return itsStokesComponent.type();
    }

  private:

    //! Initialize the object's internal parameters
    void init ();

    //! Initialize the object's internal parameters
    void init (DAL::Stokes::Component const &component);

    //! Set up the list of attributes attached to the structure
    inline void setAttributes () {
      attributes_p.clear();

      attributes_p.insert("GROUPTYPE");
      attributes_p.insert("DATATYPE");
      attributes_p.insert("STOKES_COMPONENT");
      attributes_p.insert("NOF_SUBBANDS");
      attributes_p.insert("NOF_CHANNELS");
    }
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class BF_StokesDataset -- end
  
} // Namespace DAL -- end

#endif /* BF_STOKESDATASET_H */
  
