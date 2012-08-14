/***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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

#include "HDF5Hyperslab.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab

  HDF5Hyperslab::HDF5Hyperslab ()
  {
    init();
  }
  
  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab

  /*!
    \param rank -- Rank of the dataset, i.e. the number of array axes.
  */
  HDF5Hyperslab::HDF5Hyperslab (int const &rank)
  {
    init();

    itsRank = rank;
  }
  
  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab
  
  /*!
    \param start     -- Offset of the starting element of the specified hyperslab.
    \param block     -- The size of the block selected from the dataspace.
    \param selection --  
  */
  HDF5Hyperslab::HDF5Hyperslab (std::vector<int> const &start,
				std::vector<int> const &block,
				H5S_seloper_t const &selection)
  {
    init();
    
    setStart (start);
    setBlock (block);
    itsSelection = selection;
  }
  
  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab
  
  /*!
    \param start     -- Offset of the starting element of the specified hyperslab.
    \param stride    -- Number of elements to separate each element or block to
           be selected.
    \param count     -- The number of elements or blocks to select along each
           dimension.
    \param block     -- The size of the block selected from the dataspace.
    \param selection --  
  */
  HDF5Hyperslab::HDF5Hyperslab (std::vector<int> const &start,
				std::vector<int> const &stride,
				std::vector<int> const &count,
				std::vector<int> const &block,
				H5S_seloper_t const &selection)
  {
    init();
    
    itsSelection = selection;

    setStart  (start);
    setBlock  (block);
    setCount  (count);
    setStride (stride);
  }

  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab

  /*!
    \param other -- Another HDF5Hyperslab object from which to create this new
           one.
  */
  HDF5Hyperslab::HDF5Hyperslab (HDF5Hyperslab const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  HDF5Hyperslab::~HDF5Hyperslab ()
  {
    destroy();
  }
  
  void HDF5Hyperslab::destroy ()
  {
    itsStart.clear();
    itsStride.clear();
    itsCount.clear();
    itsBlock.clear();
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  HDF5Hyperslab& HDF5Hyperslab::operator= (HDF5Hyperslab const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void HDF5Hyperslab::copy (HDF5Hyperslab const &other)
  {
    itsStart.clear();
    itsStride.clear();
    itsCount.clear();
    itsBlock.clear();

    itsRank      = other.itsRank;
    itsStart     = other.itsStart;
    itsStride    = other.itsStride;
    itsCount     = other.itsCount;
    itsBlock     = other.itsBlock;
    itsSelection = other.itsSelection;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                     setStart
  
  bool HDF5Hyperslab::setStart (std::vector<int> const &start)
  {
    bool status = true;
    int nelem   = start.size();

    /* Check if input is empty vector; in that case clear the internally stored
       value. */
    if (start.empty()) {
      itsStart.clear();
      return status;
    } else {
      /* Check if rank has been initialized. */
      if (itsRank<0) {
	itsRank = nelem;
      }
    }

    /* Process non-empty input vector. */
    if (nelem == itsRank) {
      itsStart = start;
    } else if (nelem > itsRank) {
      for (int n(0); n< itsRank; ++n) {
	itsStart[n] = start[n];
      }
    } else {
      std::cerr << "[HDF5Hyperslab::setStart] Input array has too few elements!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                    setStride

  /*!
    \param stride -- Number of elements to separate each element or block to be
           selected. If provided an empty vector, the stride will be set to 1 along
	   each axis.
    
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool HDF5Hyperslab::setStride (std::vector<int> const &stride)
  {
    bool status = true;
    int nelem   = stride.size();

    /* Check if input is empty vector; in that case clear the internally stored
       value. */
    if (stride.empty()) {
      itsStride.clear();
      return status;
    } else {
      /* Check if rank has been initialized. */
      if (itsRank<0) {
	itsRank = nelem;
      }
    }

    /* Process non-empty input vector. */
    if (nelem == itsRank) {
      itsStride = stride;
    } else if (nelem > itsRank) {
      for (int n(0); n< itsRank; ++n) {
	itsStride[n] = stride[n];
      }
    } else {
      std::cerr << "[HDF5Hyperslab::setStride] Input array has too few elements!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                     setCount
  
  bool HDF5Hyperslab::setCount (std::vector<int> const &count)
  {
    bool status = true;
    int nelem   = count.size();

    /* Check if input is empty vector; in that case clear the internally stored
       value. */
    if (count.empty()) {
      itsCount.clear();
      return status;
    } else {
      /* Check if rank has been initialized. */
      if (itsRank<0) {
	itsRank = nelem;
      }
    }

    /* Process non-empty input vector. */
    if (nelem == itsRank) {
      itsCount = count;
    } else if (nelem > itsRank) {
      for (int n(0); n< itsRank; ++n) {
	itsCount[n] = count[n];
      }
    } else {
      std::cerr << "[HDF5Hyperslab::setCount] Input array has too few elements!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                     setBlock
  
  bool HDF5Hyperslab::setBlock (std::vector<int> const &block)
  {
    bool status (true);
    int nelem = block.size();

    /* First check if the rank information has been initialized */
    if (itsRank<0) {
      itsRank = nelem;
    }
    
    if (block.empty()) {
      itsBlock.clear();
      itsBlock.resize(itsRank);
    }
    
    if (nelem == itsRank) {
      itsBlock = block;
    } else if (nelem > itsRank) {
      for (int n(0); n< itsRank; ++n) {
	itsBlock[n] = block[n];
      }
    } else {
      std::cerr << "[HDF5Hyperslab::setBlock] Input array has too few elements!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                          gap
  
  /*!
    \return gap -- The size of the gap between two subsequent blocks, i.e.
            \f$ N_{\rm stride} - N_{\rm block} \f$.
  */
  std::vector<int> HDF5Hyperslab::gap ()
  {
    std::vector<int> val (itsRank);
    
    if (itsStride.empty()) {
      for (int n(0); n<itsRank; ++n) {
	val[n] = 1;
      }
    } else {
      for (int n(0); n<itsRank; ++n) {
	val[n] = itsStride[n];
      }
    }
    
    if (itsBlock.empty()) {
      for (int n(0); n<itsRank; ++n) {
	val[n] -= 1;
      }
    } else {
      for (int n(0); n<itsRank; ++n) {
	val[n] -= itsBlock[n];
      }
    }
    
    return val;
  }
  
  //_____________________________________________________________________________
  //                                                                       setGap
  
  /*!
    \param gap -- The size of the gap between two subsequent blocks, i.e.
           \f$ N_{\rm stride} - N_{\rm block} \f$.

    \return status -- Status of the operation; return \e false in case an error
            was encountered, e.g. that there was a rank mismatch.
  */
  bool HDF5Hyperslab::setGap (std::vector<int> const &gap)
  {
    int nelem = gap.size();
    bool status (true);
    
    if (nelem == itsRank) {
      
      if (itsStride.empty()) {
	itsStride.resize(nelem);
      }
      
      if (itsBlock.empty()) {
	for (int n(0); n<nelem; ++n) {
	  itsStride[n] = 1 + gap[n];
	}
      } else {
	for (int n(0); n<nelem; ++n) {
	  itsStride[n] = itsBlock[n] + gap[n];
	}
      }
      
    } else {
      std::cerr << "[HDF5Hyperslab::setGap] Rank mismatch!" << std::endl;
      std::cerr << "-- Rank : " << itsRank << std::endl;
      std::cerr << "-- Gap  : " << gap    << std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \brief os -- Output stream to which the summary is going to be written.
  */
  void HDF5Hyperslab::summary (std::ostream &os)
  {
    os << "[HDF5Hyperslab] Summary of internal parameters." << std::endl;
    os << "-- Rank            = " << itsRank          << std::endl;
    os << "-- Start           = " << itsStart         << std::endl;
    os << "-- Stride          = " << itsStride        << std::endl;
    os << "-- Count           = " << itsCount         << std::endl;
    os << "-- Block           = " << itsBlock         << std::endl;
    os << "-- End position    = " << end()           << std::endl;
    os << "-- nof. datapoints = " << nofDatapoints() << std::endl;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         init
  
  void HDF5Hyperslab::init ()
  {
    itsRank      = -1;
    itsSelection = H5S_SELECT_SET;
    itsStart.clear();
    itsStride.clear();
    itsCount.clear();
    itsBlock.clear();
  }
  
  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param location -- HDF5 object identifier for the dataset or dataspace to
           to which the Hyperslab is going to be applied.
    \param resizeDataset -- Resize the dataset to the dimensions defined by the 
           Hyperslab?

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Hyperslab::setHyperslab (hid_t &location,
				    bool const &resizeDataset)
  {
    return setHyperslab (location,
			 itsSelection,
			 itsStart,
			 itsStride,
			 itsCount,
			 itsBlock,
			 resizeDataset);
  }

  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param datasetID     -- HDF5 object identifier for the dataset which will be
           extended, if required, to apply the hyperslab seection.
    \param dataspaceID   -- HDF5 object identifier for the dataspace to which to
           apply the hyperslab selection.
    \param resizeDataset -- Resize the dataset to the dimensions defined by the 
           Hyperslab?

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Hyperslab::setHyperslab (hid_t &datasetID,
				    hid_t &dataspaceID,
				    bool const &resizeDataset)
  {
    return setHyperslab (datasetID,
			 dataspaceID,
			 itsSelection,
			 itsStart,
			 itsStride,
			 itsCount,
			 itsBlock,
			 resizeDataset);
  }

  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param location  -- HDF5 object identifier for the dataset or dataspace to
           to which the Hyperslab is going to be applied.
    \param selection -- Selection operator to determine how the new selection is
           to be combined with the already existing selection for the dataspace.
    \param start     -- Offset of the starting element of the specified hyperslab
    \param count     -- The number of elements or blocks to select along each
           dimension.
    \param resizeDataset -- Resize the dataset to the dimensions defined by the 
           Hyperslab?

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Hyperslab::setHyperslab (hid_t &location,
  				    H5S_seloper_t const &selection,
  				    std::vector<int> const &start,
  				    std::vector<int> const &count,
  				    bool const &resizeDataset)
  {
    std::vector<int> stride;
    std::vector<int> block;
    
    return setHyperslab (location,
  			 selection,
  			 start,
  			 stride,
  			 count,
  			 block,
  			 resizeDataset);
  }
  
  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param datasetID     -- HDF5 object identifier for the dataset which will be
           extended, if required, to apply the hyperslab seection.
    \param dataspaceID   -- HDF5 object identifier for the dataspace to which to
           apply the hyperslab selection.
    \param selection -- Selection operator to determine how the new selection is
           to be combined with the already existing selection for the dataspace.
    \param start     -- Offset of the starting element of the specified hyperslab
    \param count     -- The number of elements or blocks to select along each
           dimension.
    \param resizeDataset -- Resize the dataset to the dimensions defined by the 
           Hyperslab?

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Hyperslab::setHyperslab (hid_t &datasetID,
				    hid_t &dataspaceID,
				    H5S_seloper_t const &selection,
  				    std::vector<int> const &start,
  				    std::vector<int> const &count,
  				    bool const &resizeDataset)
  {
    std::vector<int> stride;
    std::vector<int> block;
    
    return setHyperslab (datasetID,
			 dataspaceID,
  			 selection,
  			 start,
  			 stride,
  			 count,
  			 block,
  			 resizeDataset);
  }
  
  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param location  -- HDF5 object identifier for the dataset or dataspace to
           to which the Hyperslab is going to be applied.
    \param selection -- Selection operator to determine how the new selection is
           to be combined with the already existing selection for the dataspace.
    \param start     -- Offset of the starting element of the specified hyperslab
    \param stride    -- Number of elements to separate each element or block to
           be selected
    \param count     -- The number of elements or blocks to select along each
           dimension.
    \param block     -- The size of the block selected from the dataspace
    \param resizeDataset -- Resize the dataset to the dimensions defined by the 
           Hyperslab?

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Hyperslab::setHyperslab (hid_t &location,
				    H5S_seloper_t const &selection,
				    std::vector<int> const &start,
				    std::vector<int> const &stride,
				    std::vector<int> const &count,
				    std::vector<int> const &block,
				    bool const &resizeDataset)
  {
    bool status (true);

    // Check HDF5 object handler ___________________________

    if (H5Iis_valid(location)) {
      /* Check if object handler points to dataset */
      if (H5Iget_type (location) == H5I_DATASET) {
	/* Get dataspace identifier */
	hid_t dataspaceID = H5Dget_space (location);
	/* Forward function call */
	status = setHyperslab (location,
			       dataspaceID,
			       selection,
			       start,
			       stride,
			       count,
			       block,
			       resizeDataset);
	/* Release the dataspace object ID */
	H5Sclose (dataspaceID);
      } else {
	std::cerr << "[HDF5Hyperslab::setHyperslab]"
		  << " Identifier pointing to wrong object type - no dataset!"
		  << std::endl;
	return false;
      }
    } else {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Provided location not a valid"
		<< " HDF5 object!"
		<< std::endl;
      return false;
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param datasetID     -- HDF5 object identifier for the dataset which will be
           extended, if required, to apply the hyperslab seection.
    \param dataspaceID   -- HDF5 object identifier for the dataspace to which to
           apply the hyperslab selection.
    \param selection     -- Selection operator to determine how the new selection
           is to be combined with the already existing selection for the
	   dataspace.
    \param start         -- Offset of the starting element of the specified
           hyperslab.
    \param stride        -- Number of elements to separate each element or block
           to be selected
    \param count         -- The number of elements or blocks to select along each
           dimension.
    \param block         -- The size of the block selected from the dataspace
    \param resizeDataset -- Resize the dataset to the dimensions defined by the 
           Hyperslab?

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Hyperslab::setHyperslab (hid_t &datasetID,
				    hid_t &dataspaceID,
				    H5S_seloper_t const &selection,
				    std::vector<int> const &start,
				    std::vector<int> const &stride,
				    std::vector<int> const &count,
				    std::vector<int> const &block,
				    bool const &resizeDataset)
  {
    bool status = true;

    // Check HDF5 object handlers __________________________

    /* Check the dataset */
    if (H5Iis_valid(datasetID)) {
      if (H5Iget_type (datasetID) != H5I_DATASET) {
	std::cerr << "[HDF5Hyperslab::setHyperslab]"
		  << " Identifier pointing to wrong object type - no dataset!"
		  << std::endl;
	return false;
      }
    } else {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Provided location not a valid"
		<< " HDF5 object!"
		<< std::endl;
      return false;
    }

    /* Check the dataspace */
    if (H5Iis_valid(dataspaceID)) {
      if (H5Iget_type (dataspaceID) != H5I_DATASPACE) {
	std::cerr << "[HDF5Hyperslab::setHyperslab]"
		  << " Identifier pointing to wrong object type - no dataspace!"
		  << std::endl;
	return false;
      }
    } else {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Provided location not a valid"
		<< " HDF5 object!"
		<< std::endl;
      return false;
    }

    // Check Hyperslab parameters __________________________

    unsigned int nelem;
    std::vector<hsize_t> shape;
    bool haveStride (true);
    bool haveCount (true);
    bool haveBlock (true);
    herr_t h5error;

    /* Get shape and rank of the dataspace */
    HDF5Dataspace::shape (datasetID,shape);
    nelem = shape.size();
    
    /* Start position m1*/
    if (start.size() != nelem) {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Parameter mismatch: start - shape."
		<< std::endl << std::flush;
      std::cerr << "-- start = " << start << std::endl << std::flush;
      std::cerr << "-- shape = " << shape << std::endl << std::flush;
      return false;
    }
    
    if (stride.size() != nelem || stride.empty()) {
      haveStride = false;
    }
    
    if (count.size() != nelem || count.empty()) {
      haveCount = false;
    }

    if (block.size() != nelem || block.empty()) {
      haveBlock = false;
    }

    // Adjust the size of the dataset ______________________

    try {
      std::vector<hsize_t> endHyperslab = end (start,stride,count,block);
      bool extendDataset (false);
      hsize_t tmpSize [nelem];
      
      for (unsigned int n(0); n<nelem; ++n) {
	if (endHyperslab[n]>shape[n]) {
	  tmpSize[n] = endHyperslab[n]+1;
	  extendDataset=true;
	} else {
	  tmpSize[n] = shape[n];
	}
      }
      
      if (extendDataset && resizeDataset) {
	/* Feedback */
	std::cout << "-- Extending dataset : " << shape << " -> " 
		  << toString(tmpSize,nelem) << std::endl;
	/* Close the dataspace */
	h5error = H5Sclose (dataspaceID);
	/* Extend the dataset */
	h5error = H5Dset_extent (datasetID, tmpSize);
	/* Repoen the dataspace */
	dataspaceID = H5Dget_space(datasetID);
	/* Check the error code */
	if (h5error<0) {
	  std::cerr << "[HDF5Hyperslab::setHyperslab] Error extending dataset!"
		    << std::endl;
	  status = false;
	}
      }
    } catch (std::string message) {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Error extending dataset!"
		<< std::endl;
      status = false;
    }
    
    // Set up the hyperslab parameters _____________________
    
    hsize_t rank (shape.size());
    hsize_t tmpStart [rank];
    hsize_t tmpStride[rank];
    hsize_t tmpCount [rank];
    hsize_t tmpBlock [rank];
    
    /* Copy the input parameters to be passed on */
    for (unsigned int n(0); n<nelem; ++n) {
      tmpStart[n]  = start[n];
    }
    
    if (haveStride) {
      for (unsigned int n(0); n<nelem; ++n) {
	tmpStride[n] = stride[n];
      }
    } else {
      for (unsigned int n(0); n<nelem; ++n) {
	tmpStride[n] = 1;
      }
    }
    
    if (haveCount) {
      for (unsigned int n(0); n<nelem; ++n) {
	tmpCount[n] = count[n];
      }
    } else {
      for (unsigned int n(0); n<nelem; ++n) {
	tmpCount[n] = 1;
      }
    }

    if (haveBlock) {
      for (unsigned int n(0); n<nelem; ++n) {
	tmpBlock[n] = block[n];
      }
    } else {
      for (unsigned int n(0); n<nelem; ++n) {
	tmpBlock[n] = 1;
      }
    }
    
    // Select hyperslab from dataspace _____________________
    
    /* Assign the Hyperslab selection to the dataspace. */
    h5error = H5Sselect_hyperslab (dataspaceID,
				   selection,
				   tmpStart,      // start
				   tmpStride,     // stride
				   tmpCount,      // count
				   tmpBlock);     // block
    
    if (h5error<0) {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Error selecting hyperslab!"
		<< std::endl << std::flush;
      status = false;
    } else {
      /* Check if the selection was valid */
      htri_t errorCode;
      status = checkSelectionValid (dataspaceID,errorCode);
#ifdef DAL_DEBUGGING_MESSAGES
      /* Get the bounding of the selection */
      std::vector<hsize_t> posStart;
      std::vector<hsize_t> posEnd;
      getBoundingBox (dataspaceID, posStart, posEnd);
      std::cout << "-- Bounding box = " << posStart << " .. " << posEnd << std::endl;
#endif
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                nofDatapoints
  
  unsigned int HDF5Hyperslab::nofDatapoints ()
  {
    return nofDatapoints (itsCount,
			  itsBlock);
  }
  
  //_____________________________________________________________________________
  //                                                                nofDatapoints
  
  /*!
    \param count     -- The number of elements or blocks to select along each
           dimension.
    \param block     -- The size of the block selected from the dataspace.

    \return nofDatapoints -- The number of datapoints returned using a given 
            hyperslab.
  */
  unsigned int HDF5Hyperslab::nofDatapoints (std::vector<int> const &count,
					     std::vector<int> const &block)
  {
    unsigned int sizeCount = count.size();
    unsigned int sizeBlock = block.size();
    unsigned int nelem (1);
    
    if (sizeCount) {
      if (sizeBlock) {
	if (sizeCount == sizeBlock) {
	  for (unsigned int n(0); n<sizeCount; ++n) {
	    nelem *= count[n]*block[n];
	  }
	} else {
	  std::cerr << "[HDF5Hyperslab::nofDatapoints] Size mismatch!" << std::endl;
	  std::cerr << "-- shape(count) = " << sizeCount << std::endl;
	  std::cerr << "-- shape(block) = " << sizeBlock << std::endl;
	}
      } else {
	for (unsigned int n(0); n<sizeCount; ++n) {
	  nelem *= count[n];
	}
      }
    }  //  end : if (sizeCount)
    else {
      if (sizeBlock) {
	for (unsigned int n(0); n<sizeBlock; ++n) {
	  nelem *= block[n];
	}
      } else {
	nelem = 0;
      }
    }
    
    return nelem;
  }
  
  //_____________________________________________________________________________
  //                                                                          end

  std::vector<hsize_t> HDF5Hyperslab::end ()
  {
    return end (itsStart,
		itsStride,
		itsCount,
		itsBlock);
  }

  //_____________________________________________________________________________
  //                                                                          end

  /*!
    \param start     -- Offset of the starting element of the specified hyperslab,
           \f$ N_{\rm start} \f$ .
    \param stride    -- Number of elements to separate each element or block to
           be selected, \f$ N_{\rm stride} \f$ .
    \param count     -- The number of elements or blocks to select along each
           dimension, \f$ N_{\rm count} \f$ .
    \param block     -- The size of the block selected from the dataspace,
           \f$ N_{\rm block} \f$ .
    
    \return end -- The offset of the last element of the specified hyperslab; given
            above input parameters the range of elements defined by the hyperslab
	    becomes
	    \f[
	    N_{\rm end} \ = \ N_{\rm start} + N_{\rm block} \cdot n_{\rm count} - 1
	    + (N_{\rm block}-N_{\rm stride}) \cdot (N_{\rm count}-1)
	    \f]
	    For this we have two special cases:
	    \f[
	    N_{\rm end} \ = \ \left\{  \begin{array}{lcl}
	    N_{\rm start} + N_{\rm block} -1 & \hbox{if} &  N_{\rm count} \equiv 1 \\[3mm]
	    N_{\rm start} + \left( 2 N_{\rm block} - 1 \right) \cdot N_{\rm count}
	    - N_{\rm block} & \hbox{if} &  N_{\rm stride} \equiv 1
	    \end{array} \right.
	    \f]
  */
  std::vector<hsize_t> HDF5Hyperslab::end (std::vector<int> const &start,
					   std::vector<int> const &stride,
					   std::vector<int> const &count,
					   std::vector<int> const &block)
  {
    unsigned int sizeStart  = start.size();
    unsigned int sizeStride = stride.size();
    unsigned int sizeCount  = count.size();
    unsigned int sizeBlock  = block.size();
    std::vector<int> tmpStride (sizeStart);
    std::vector<int> tmpCount (sizeStart);
    
    std::vector<hsize_t> pos;
    
    // Check input parameters ____________________
    
    if (sizeStart  != sizeBlock) {
      std::cerr << "[HDF5Hyperslab::end] Size mismatch!" << std::endl;
      std::cerr << "-- start = " << start << std::endl;
      std::cerr << "-- block = " << block << std::endl;
      return pos;
    }
    
    if (sizeStride != sizeStart || stride.empty()) {
      tmpStride = std::vector<int> (sizeStart,1);
    } else {
      tmpStride = stride;
    }
    
    if (sizeCount != sizeStart || count.empty()) {
      tmpCount = std::vector<int> (sizeStart,1);
    } else {
      tmpCount = count;
    }
    
    // Compute end position ______________________

    pos.resize(sizeStart);
    
    for (unsigned int n(0); n<sizeStart; ++n) {
      pos[n] = start[n];
      pos[n] += tmpCount[n]*block[n] - 1 + (block[n]-tmpStride[n])*(tmpCount[n]-1);
    }
    
    return pos;
  }

  //_____________________________________________________________________________
  //                                                          checkSelectionValid

  /*!
    \param location   -- HDF5 object identifier for the dataspace to which a
           hyperslab selection was performed.
    \retval errorCode -- Returns a positive value, for TRUE, if the selection is
            contained within the extent or 0 (zero), for FALSE, if it is not.
	    Returns a negative value on error conditions such as the selection
	    or extent not being defined.
    \return status -- Returns \e true in case the selection was valid, \e false
            otherwise.
  */
  bool HDF5Hyperslab::checkSelectionValid (hid_t const &location,
					   htri_t &errorCode)
  {
    bool status = true;
    
    if (H5Iis_valid(location)) {
      // check if the selection is valid
      errorCode = H5Sselect_valid (location);
      // process the returned error code
      if (errorCode>0) {
	status = true;
      }
      else if (errorCode==0) {
	std::cerr << "[HDF5Hyperslab::checkSelectionValid]"
		  << " Selection is NOT contained within the extend of the dataset!"
		  << std::endl;
	status = false;
      } else if (errorCode<0) {
	std::cerr << "[HDF5Hyperslab::checkSelectionValid]"
		  << " Selection parameters NOT defined properly!"
		  << std::endl;
	status = false;
      }
    } else {
      std::cerr << "[HDF5Hyperslab::checkSelectionValid]"
		<< " Location is not valid HDF5 object!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                               getBoundingBox

  /*!
    \param location -- HDF5 object identifier for the dataspace to which a
           hyperslab selection was performed.
    \param start    -- Starting coordinates of the bounding box.
    \param end      -- Ending coordinates of the bounding box, i.e., the
           coordinates of the diagonally opposite corner.
  */
  bool HDF5Hyperslab::getBoundingBox (hid_t const &location,
				      std::vector<hsize_t> &start,
				      std::vector<hsize_t> &end)
  {
    bool status (true);

    if (H5Iis_valid(location)) {

      bool inputIsDataset   = false;
      H5I_type_t objectType = H5Iget_type (location);
      unsigned int rank;
      std::vector<hsize_t> shape;
      hid_t dataspaceID;
      herr_t h5error;
      
      if (objectType == H5I_DATASPACE) {
	dataspaceID = location;
      }
      else if (objectType == H5I_DATASET) {
	dataspaceID    = H5Dget_space (location);
	inputIsDataset = true;
      } else {
	std::cerr << "[HDF5Hyperslab::getBoundingBox]"
		  << " Invalid type of HDF5 object!" << std::endl;
	return false;
      }
      
      /* Retrieve the shape of the dataspace */
      HDF5Dataspace::shape (dataspaceID,shape);
      if (shape.empty()) {
	std::cerr << "[HDF5Hyperslab::getBoundingBox]"
		  << " Failed to retrieve shape of dataspace!." << std::endl;
	return false;
      } else {
	rank = shape.size();
      }
      
      hsize_t *tmpStart = new hsize_t[rank];
      hsize_t *tmpEnd   = new hsize_t[rank];

      h5error = H5Sget_select_bounds (dataspaceID,
      				      tmpStart,
      				      tmpEnd);

      if (h5error<0) {
	std::cerr << "" << std::endl;
      } else {
	start.resize(rank);
	end.resize(rank);

	for (unsigned int n(0); n<rank; ++n) {
	  start[n] = tmpStart[n];
	  end[n]   = tmpEnd[n];
	}
      }

      /* Release allocated memory */
      delete [] tmpStart;
      delete [] tmpEnd;
      /* Release HDF5 object handle */
      if (inputIsDataset) {
	H5Sclose(dataspaceID);
      }
      
    } else {
      std::cerr << "[HDF5Hyperslab::getBoundingBox]"
		<< " Location is not valid HDF5 object!"
		<< std::endl;
      status = false;
    }

    return status;
  }
  
  } // Namespace DAL -- end
