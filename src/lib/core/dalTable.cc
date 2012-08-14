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

#include <core/dalTable.h>

namespace DAL {
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    dalFilter
  
  dalTable::dalTable()
    : dalObjectBase()
  {
    init ();
  }

  //_____________________________________________________________________________
  //                                                                     dalTable

  /*!
    \param filetype -- File type descriptor, passed as \c dalFileType object.
  */
  dalTable::dalTable (dalFileType const &filetype)
    : dalObjectBase(filetype)
  {
    init ();
  }

  //_____________________________________________________________________________
  //                                                                     dalTable
  
  /*!
    \param filetype -- File type descriptor, passed as \c dalFileType::Type.
  */
  dalTable::dalTable (dalFileType::Type const &filetype)
    : dalObjectBase(dalFileType(filetype))
  {
    init ();
  }

  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                   ~dalFilter
  
  dalTable::~dalTable()
  {
#ifdef DAL_WITH_CASA
    if (itsFiletype.type() == dalFileType::CASA_MS) {
      delete itsCasaTable;
      delete itsCasaColumn;
    }
#endif
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init
  
  void dalTable::init ()
  {
    file           = NULL;
    itsFileID      = 0;
    itsTableID     = 0;
    nfields        = 0;
    nofRecords_p   = 0;
    status         = 0;
    itsFirstRecord = true;
    itsFilter      = dalFilter();

    columns.clear();
    
    switch (itsFiletype.type()) {
#ifdef DAL_WITH_CASA
    case dalFileType::CASA_MS:
      {
	itsCasaTable = new casa::Table;
      }
      break;
#endif
    case dalFileType::FITS:
      break;
    case dalFileType::HDF5:
      break;
    default:
      break;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- The output stream to which the summary is going to be written
  */
  void dalTable::summary(std::ostream &os)
  {
    os << "\n[dalTable] Summary of object properties"  << endl;

    os << "-- Table name    = " << itsName            << std::endl;
    os << "-- Table type    = " << itsFiletype.name() << std::endl;
    os << "-- nof. rows     = " << getNumberOfRows()  << std::endl;
    os << "-- nof. columns  = " << nofColumns()       << std::endl;
    os << "-- nof. records  = " << nofRecords()       << std::endl;
    os << "-- nof. fields   = " << nofFields()        << std::endl;
    
    
    /* If the table contains a non-zero number of columns, list their names.
     */
    if (!columns.empty()) {
      os << "-- Column names  = [";
      for (unsigned int n=0; n<columns.size(); ++n)
	{
	  os << " " << columns[n].name();
	}
      os << " ]" << std::endl;
    }
    
    /* If the table is working with HDF5 as back-end, provide a summary of the
     * identifiers and properties.
     */
    if (itsFiletype.type()==dalFileType::HDF5) {
      os << "-- HDF5 file ID  = " << itsFileID    << std::endl;
      os << "-- HDF5 table ID = " << itsTableID   << std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                    getColumn
  
  /*!
    \param colname The name of the column to retrieve.
    \return Pointer to dalColumn object.
  */
  dalColumn * dalTable::getColumn ( std::string colname )
  {

    switch (itsFiletype.type()) {
    case dalFileType::CASA_MS:
      {
#ifdef DAL_WITH_CASA
      // using the dalColumn class
      dalColumn * lclcol = NULL;
      lclcol = new dalColumn( *itsCasaTable, colname );
      return lclcol;
#else
      std::cerr << "ERROR: casacore not installed" << endl;
      // prevent unsused var build warning when casacore isn't installed
      colname = colname;
      return NULL;
#endif
      }
      break;
    default:
      {
	std::cerr << "[dalTable::getColumn] Operation not supported for filetype ("
		  << itsFiletype.name() << ")."
		  << std::endl;
	return NULL;
      }
      break;
    };
    
  }
  
  //_____________________________________________________________________________
  //                                                            getColumn_Float32
  
  /*!
    \brief Get a column object from a table.
    
    Gets a column object from a table object.
    
    \param colname The name of the column you want to get from the table.
    \return dalColumn Pointer to a column object.
  */
  dalColumn * dalTable::getColumn_Float32( std::string colname )
  {

    switch (itsFiletype.type()) {
#ifdef DAL_WITH_CASA
    case dalFileType::CASA_MS:
      {
      dalColumn * lclcol = NULL;
      lclcol = new dalColumn( *itsCasaTable, colname );
      return lclcol;
      }
      break;
#endif
    case dalFileType::HDF5:
      {
	dalColumn * lclcol;
	lclcol = new dalColumn (itsFileID,
				itsTableID,
				dalFileType(dalFileType::HDF5),
				itsName,
				colname,
				dal_FLOAT);
	
	return lclcol;
      }
      break;
    default:
      {
	std::cerr << "[dalTable::getColumn_Float32]"
		  << " Operation not supported for filetype ("
		  << itsFiletype.name() << ")."
		  << std::endl;
	return NULL;
      }
      break;
    };

  }
  
  //_____________________________________________________________________________
  //                                                     getColumn_complexFloat32
  
  /*!
    \brief Get a column object from a table.
    
    Gets a column object from a table object.
    
    \param colname The name of the column you want to get from the table.
    \return dalColumn Pointer to a column object.
  */
  dalColumn * dalTable::getColumn_complexFloat32( std::string colname )
  {

    switch (itsFiletype.type()) {
#ifdef DAL_WITH_CASA
    case dalFileType::CASA_MS:
      {
        dalColumn * lclcol = NULL;
        lclcol = new dalColumn( *itsCasaTable, colname );
        return lclcol;
      }
      break;
#endif
    case dalFileType::HDF5:
      {
        dalColumn * lclcol;
        lclcol = new dalColumn (itsFileID,
				itsTableID,
				dalFileType(dalFileType::HDF5),
				itsName,
				colname,
                                dal_COMPLEX);

        return lclcol;
      }
      break;
    default:
      {
	std::cerr << "[dalTable::getColumn_ComplexFloat32]"
		  << " Operation not supported for filetype ("
		  << itsFiletype.name() << ")."
		  << std::endl;
	return NULL;
      }
      break;
    };

  }

  //_____________________________________________________________________________
  //                                                       getColumn_complexInt16
  
  /*!
  \brief Get a column object from a table.

   Gets a column object from a table object.

   \param colname The name of the column you want to get from the table.
   \return dalColumn Pointer to a column object.
  */
  dalColumn * dalTable::getColumn_complexInt16( std::string colname )
  {

    switch (itsFiletype.type()) {
#ifdef DAL_WITH_CASA
    case dalFileType::CASA_MS:
      {
        dalColumn * lclcol = NULL;
        lclcol = new dalColumn( *itsCasaTable, colname );
        return lclcol;
      }
      break;
#endif
    case dalFileType::HDF5:
      {
        dalColumn * lclcol;
        lclcol = new dalColumn (itsFileID,
				itsTableID,
				dalFileType(dalFileType::HDF5),
				itsName,
				colname,
                                dal_COMPLEX_SHORT);
        return lclcol;
      }
      break;
    default:
      {
	std::cerr << "[dalTable::getColumn_complexInt16]"
		  << " Operation not supported for filetype ("
		  << itsFiletype.name() << ")."
		  << std::endl;
	return NULL;
      }
      break;
    };

  }

  //_____________________________________________________________________________
  //                                                                    setFilter
  
  /*!
  \brief Set a filter on the table.

  Sets a filter on the table so that when it is opened, it will only
  contain a subset of information contained within the full dataset table.

  \param columns A comma-separated list of columns you wish read from the
                 table.
  */
  void dalTable::setFilter (std::string const &columns)
  {
    itsFilter = dalFilter();
    itsFilter.setFiletype(itsFiletype);
    itsFilter.setFilter(columns);
  }

  //_____________________________________________________________________________
  //                                                                    setFilter
  
  /*!
    \brief Set a filter on the table.
    
    Sets a filter on the table so that when it is opened, it will only
    contain a subset of information contained within the full dataset table.
    
    \param columns A comma-separated list of columns you wish read from the
           table.
    \param conditions The condition you wish to apply to the columns in the
           filter.  For example: "TIME>100".
  */
  void dalTable::setFilter (std::string const &columns,
			    std::string const &conditions)
  {
    itsFilter.setFiletype(itsFiletype);
    itsFilter.setFilter(columns,conditions);
  }
  
  //_____________________________________________________________________________
  //                                                                getColumnData

  /*!
    \param colname The name of the column to retrieve.
    \return A generic pointer to the column data.
   */
  void * dalTable::getColumnData( std::string colname )
  {
    if (itsFiletype.type()==dalFileType::CASA_MS)
      {
#ifdef DAL_WITH_CASA
        // using the dalColumn class
        dalColumn lclcol;
        lclcol = dalColumn( *itsCasaTable, colname );
        lclcol.getType();
#ifdef DAL_DEBUGGING_MESSAGES
        if ( lclcol.isScalar() )
          std::cerr << colname << " is SCALAR" << endl;
        if ( lclcol.isArray() )
          std::cerr << colname << " is ARRAY" << endl;
#endif
        casa::uInt nrow;
        nrow = itsCasaTable->nrow();

        // create a column object
        casa::ROTableColumn column( *itsCasaTable, colname );

        // get the column descriptor
        casa::ColumnDesc col_desc = column.columnDesc();

        std::cerr << "-------\n" << col_desc.name() << "\n-------" << endl;
        std::cerr << "Is the column an array? ";
        casa::Bool isarray = col_desc.isArray();
        if ( casa::True == isarray )
          {
#ifdef DAL_DEBUGGING_MESSAGES
            std::cerr << "YES" << endl;
#endif
            casa::uInt ndimcol;
            ndimcol = column.ndimColumn();
            casa::IPosition shape = column.shape(1);
#ifdef DAL_DEBUGGING_MESSAGES
            std::cerr << "  Number of global dims: " << ndimcol << endl;
            std::cerr << "  Shape: " << shape << endl;
#endif
            switch ( col_desc.dataType() )
              {
              case casa::TpComplex:
              {
#ifdef DAL_DEBUGGING_MESSAGES
                std::cerr << "Data type is Complex." << endl;
#endif
                casa::ROArrayColumn<casa::Complex>
                arcolumn( *itsCasaTable, colname );
                int polarization = 0;
                casa::IPosition start (2,polarization,0)/*, length (1,1)*/;
                casa::Slicer slicer (start/*, length*/);
                itsArrayComplex = arcolumn.getColumn( slicer );
#ifdef DAL_DEBUGGING_MESSAGES
                std::cerr << "number of dims: " << itsArrayComplex.ndim() << endl;
                std::cerr << "shape: " << itsArrayComplex.shape() << endl;
                std::cerr << "size: " << itsArrayComplex.size() << endl;
#endif
                std::vector< std::complex< float > > valvec;
                itsArrayComplex.tovector( valvec );
#ifdef DAL_DEBUGGING_MESSAGES
                std::cerr << valvec[0] << valvec[1] << valvec[2] << endl;
                std::cerr << "vector size: " << valvec.size() << endl;
                std::cerr << "Polarization number: " << polarization << endl;
#endif
                return itsArrayComplex.data();
              }
              break;
              case casa::TpDComplex:
                std::cerr << "Data type is DComplex." << endl;
                break;
              case casa::TpBool:
                std::cerr << "Data type is Boolean." << endl;
                break;
              case casa::TpChar:
                std::cerr << "Data type is Char." << endl;
                break;
              case casa::TpShort:
                std::cerr << "Data type is Short." << endl;
                break;
              case casa::TpFloat:
                std::cerr << "Data type is Float." << endl;
                break;
              case casa::TpDouble:
              {
                std::cerr << "Data type is Double." << endl;
                casa::ROArrayColumn<casa::Double>
                arcolumn( *itsCasaTable, colname );
                int cell = 0;
                casa::IPosition start (1,cell)/*, length (1,1)*/;
                casa::Slicer slicer (start/*, length*/);
                itsArrayDouble = arcolumn.getColumn( slicer );
#ifdef DAL_DEBUGGING_MESSAGES
                std::cerr << "number of dims: " << itsArrayDouble.ndim() << endl;
                std::cerr << "shape: " << itsArrayDouble.shape() << endl;
                std::cerr << "size: " << itsArrayDouble.size() << endl;
#endif
                std::vector<double> valvec;
                itsArrayDouble.tovector( valvec );
#ifdef DAL_DEBUGGING_MESSAGES
                std::cerr << "vector size: " << valvec.size() << endl;
                std::cerr << "Data from cell number: " << cell << endl;
#endif
                return itsArrayDouble.data();
              }
              break;
              case casa::TpString:
                std::cerr << "Data type is String." << endl;
                break;
              case casa::TpInt:
              {
#ifdef DAL_DEBUGGING_MESSAGES
                std::cerr << "Data type is Int." << endl;
#endif
                int * value = new int[100];
                for (int ii=0; ii<100; ii++)
                  {
                    column.getScalar(ii,value[ii]);
                  }
                return value;
              }
              break;
              default:
                std::cerr << "Datatype not recognized." << endl;
              }
          }
        else
          std::cerr << "NO" << endl;

        if (col_desc.isScalar())
          {
            std::cerr << "isScalar" << endl;
            switch ( col_desc.dataType() )
              {
              case casa::TpComplex:
                std::cerr << "Data type is Complex." << endl;
                break;
              case casa::TpBool:
                std::cerr << "Data type is Boolean." << endl;
                break;
              case casa::TpChar:
                std::cerr << "Data type is Char." << endl;
                break;
              case casa::TpShort:
                std::cerr << "Data type is Short." << endl;
                break;
              case casa::TpFloat:
                std::cerr << "Data type is Float." << endl;
                break;
              case casa::TpDouble:
              {
                std::cerr << "Data type is Double." << endl;
                double * value = new double[100];
                for (int ii=0; ii<100; ii++)
                  {
                    column.getScalar(ii,value[ii]);
                  }
                return value;
              }
              break;
              case casa::TpString:
                std::cerr << "Data type is String." << endl;
                break;
              case casa::TpInt:
              {
                std::cerr << "Data type is Int." << endl;
                int * value = new int[100];
                for (int ii=0; ii<100; ii++)
                  {
                    column.getScalar(ii,value[ii]);
                  }
                return value;
              }
              break;
              default:
                std::cerr << "Datatype not recognized." << endl;
              }
          }
        else if (col_desc.isTable())
          {
            std::cerr << "isTable" << endl;
          }

        casa::TableRecord table_rec = column.keywordSet();
#ifdef DAL_DEBUGGING_MESSAGES
        std::cerr << "Number of fields: " << table_rec.nfields() << endl;
        std::cerr << "Datatype: " << col_desc.dataType() << endl;
#endif
        return NULL;
#else
        // prevent unused variable build warning when casacore isn't installed
        colname = colname;
        std::cerr << "ERROR: casacore not installed." << endl;
        return NULL;
#endif
      }
    else
      {
        std::cerr << "dalTable::getColumnData operation not supported for type "
                  << itsFiletype.name() << endl;
        return NULL;
      }
  }

  //_____________________________________________________________________________
  //                                                                      getName
  
  std::string dalTable::tableName()
  {

    switch (itsFiletype.type()) {
    case dalFileType::CASA_MS:
      {
#ifdef DAL_WITH_CASA
	return itsCasaTable->tableName();
#else
        return std::string ("");
#endif
      }
      break;
    default:
      {
	std::cerr << "[dalTable::tableName]"
		  << " Operation not supported for filetype ("
		  << itsFiletype.name() << ")."
		  << std::endl;
        return std::string ("");
      }
      break;
    };

  }

#ifdef DAL_WITH_CASA

  //_____________________________________________________________________________
  //                                                                    openTable
  
  /*!
    \param tablename -- The name of the table you want to open.
    \return status   -- Status of the operation; return \e false in case an error
            was encountered.
  */
  bool dalTable::openTable (std::string const &tablename)
  {
    bool status = true;

    switch (itsFiletype.type()) {
#ifdef DAL_WITH_CASA
    case dalFileType::CASA_MS:
      {
	if ( itsFilter.isSet() ) {
	  try {
	    *itsCasaTable = casa::Table( tablename );
	    *itsCasaTable = casa::tableCommand( itsFilter.get(),
						*itsCasaTable );
	  }
	  catch (casa::AipsError x) {
	    std::cerr << "ERROR: " << x.getMesg() << endl;
	  }
	}
	else {
	  try {
	    *itsCasaTable = casa::Table( tablename );
	  }
	  catch (casa::AipsError x) {
	    std::cerr << "ERROR: " << x.getMesg() << endl;
	  }
	}
      }
      break;
#endif
    default:
      {
	std::cerr << "[dalTable::openTable]"
		  << " Operation not supported for filetype ("
		  << itsFiletype.name() << ")."
		  << std::endl;
	status = false;
      }
      break;
    };
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                    openTable
  
  /*!
    \param tablename The name of the table you want to open.
    \param reader The CASA MS table reader object.
  */
  void dalTable::openTable (std::string const &tablename,
                            casa::MSReader * reader)
  {
    if (itsFiletype.type()==dalFileType::CASA_MS) {
#ifdef DAL_WITH_CASA
      try {
	*itsCasaTable = reader->table( tablename );
      }
      catch (casa::AipsError x) {
	std::cerr << "ERROR: " << x.getMesg() << endl;
      }
#else
      std::cerr << "CASA support not enabled.\n";
#endif
    }
    else {
      std::cerr << "dalTable::openTable operation not supported for type "
		<< itsFiletype.name() << endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                    openTable
  
  /*!
    \param tablename -- Name of the table
    \param reader    -- MeasurementSet reader
    \param filter    -- Access filter to be applied to the table
  */
  void dalTable::openTable( std::string const &tablename,
                            casa::MSReader * reader,
                            dalFilter const &filter)
  {
    if (itsFiletype.type()==dalFileType::CASA_MS) {
#ifdef DAL_WITH_CASA
      try {
	*itsCasaTable = reader->table( tablename );
	itsFilter     = filter;
	*itsCasaTable = casa::tableCommand( itsFilter.get(),
					    *itsCasaTable );
      }
      catch (casa::AipsError x) {
	std::cerr << "ERROR: " << x.getMesg() << endl;
      }
#else
      std::cerr << "ERROR: CASA support not enabled.\n";
#endif
    }
    else {
      std::cerr << "ERROR: dalTable::openTable operation not supported for type "
		<< itsFiletype.name() << endl;
    }
  }
#endif
  
  //_____________________________________________________________________________
  //                                                                    openTable
  
  /*!
    \param voidfile  -- A pointer to the file you want to open.
    \param tablename -- The name of the table you want to open.
    \param groupname -- The name of the group containing the table you want
           to open.
  */
  void dalTable::openTable( void * voidfile,
                            std::string const &tablename,
                            std::string const &groupname)
  {
    if (itsFiletype.type()==dalFileType::HDF5) {
      itsName = groupname + '/' + tablename;
      hid_t * lclfile = (hid_t*)voidfile; // H5File object
      file = lclfile;
      itsFileID = *lclfile;  // get the file handle
      
      itsTableID = H5Dopen ( itsFileID, itsName.c_str(), H5P_DEFAULT );
    }
    else {
      std::cerr << "dalTable::openTable operation not supported for type "
		<< itsFiletype.name() << endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                 printColumns
  
  /*!
    Print a list of the columns contained in the table.
  */
  void dalTable::printColumns()
  {
    if (itsFiletype.type()==dalFileType::HDF5) {
      size_t * field_sizes = NULL;
      size_t * field_offsets = NULL;
      size_t * size_out = NULL;
      
      // retrieve the input fields needed for the append_records call
      H5TBget_table_info ( itsFileID, itsName.c_str(), &nfields, &nofRecords_p );
      
      field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
      field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
      size_out = (size_t*)malloc( sizeof(size_t) );
      
      /* Alocate space */
      itsFieldNames = (char**)malloc( sizeof(char*) * (size_t)nfields );
      for ( unsigned int i = 0; i < nfields; i++)
	{
	  itsFieldNames[i]=(char*)malloc(sizeof(char) * MAX_COL_NAME_SIZE );
	}
      status = H5TBget_field_info( itsFileID, itsName.c_str(), itsFieldNames,
				   field_sizes, field_offsets, size_out );
      
      for (unsigned int ii=0; ii<nfields; ii++)
	{
	  std::cerr << std::setw(17) << itsFieldNames[ii];
	  free(itsFieldNames[ii]);
	}
      free(itsFieldNames);
      std::cerr << endl;
    }
    else if (itsFiletype.type()==dalFileType::CASA_MS) {
#ifdef DAL_WITH_CASA
      casa::TableDesc td = itsCasaTable->tableDesc();
      std::cerr << td.columnNames() << endl;
#else
      std::cerr << "ERROR: CASA support not enabled.\n";
#endif
    }
    else {
      std::cerr << "Operation not yet supported for type " << itsFiletype.name()
		<< ".  Sorry.\n";
    }
  }
  
  //_____________________________________________________________________________
  //                                                                  createTable
  
  /*!
    Create a new table in the dataset, possibly within a group.  This
    function is usually called by dalDataset, and not by the developer.

    \param voidfile A pointer to the file.
    \param tablename The name of the table you want to create.
    \param groupname The name of the group you where you want to create
           the table.
  */
  void dalTable::createTable (void * voidfile,
                              std::string const &tablename,
                              std::string groupname )
  {
    if (itsFiletype.type()==dalFileType::HDF5)
      {
        // It is necessary to have at least one column for table creation
        // so we create a dummy column that will be deleted when real
        // columns are added
        //

        itsName = groupname + '/' + tablename;// set the private class variable: name
        // cast the voidfile to an hdf5 file
        hid_t * lclfile = (hid_t*)voidfile; // H5File object
        file = lclfile;
        itsFileID = *lclfile;  // get the file handle

        const char * lclfield_names[1]  = { "000dummy000" };
        size_t dst_offset[1] = { 0  };
        hid_t field_type[1] = { H5T_NATIVE_INT };
        hsize_t chunk_size = (hsize_t)CHUNK_SIZE;
        int compress  = 0;  // 0=off 1=on
        int * fill    = NULL;
        fill          = new int[1];
        fill[0]       = 0;

        typedef struct Particle
          {
            int dummy;
          } Particle;
        Particle data[1] = {{0}};
        size_t dst_size =  sizeof(Particle);

        dst_size = sizeof( Particle );
        dst_offset[0] = HOFFSET( Particle, dummy );

	std::string pathToTable = groupname + '/' + tablename;

        status = H5TBmake_table (pathToTable.c_str(),
				 itsFileID,
				 pathToTable.c_str(),
                                 1,
				 1,
				 dst_size,
				 lclfield_names,
                                 dst_offset,
				 field_type,
				 chunk_size,
                                 fill,
				 compress,
				 data);
        delete [] fill;
        fill = NULL;
        itsTableID = H5Dopen ( itsFileID, tablename.c_str(), H5P_DEFAULT );
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << itsFiletype.name()
		  << ".  Sorry.\n";
      }

  }

  //_____________________________________________________________________________
  //                                                            h5addColumn_setup

  /*!
    \param column_name -- Name of the column to be added
    \param removedummy -- Remove dummy column
  */
  bool dalTable::h5addColumn_setup (std::string const &column_name,
                                    bool &removedummy)
  {
    herr_t h5err (0);

    /*________________________________________________________________
      Make sure the column name isn't blank; is this is not the case
      retrieve basic table information.
    */

    if ( 0 == column_name.length() ) {
      std::cerr << "WARNING: Trying to add column without a name.\n";
      return false;
    } else {
      /* 
	 Returns a non-negative value if successful; otherwise returns
	 a negative value. 
      */
      h5err = H5TBget_table_info (itsFileID,
				  itsName.c_str(),
				  &nfields,
				  &nofRecords_p);
      if (h5err<0) {
	std::cerr << "[dalTable::h5addColumn_setup]"
		  << " Failed to retrieve information about table!" 
		  << std::endl;
	return false;
      }
    }
    
    /*________________________________________________________________
      Allocate space for the column/field names and retrieve them from
       the table
    */

    itsFieldNames = (char**)malloc( nfields * sizeof(char*) );

    for (unsigned int ii=0; ii<nfields; ii++) {
      itsFieldNames[ii] = (char*)malloc(MAX_COL_NAME_SIZE * sizeof(char));
    }
    
    /*________________________________________________________________
      Retrieve information about targetted field within the table.
      
      Returns a non-negative value if successful; otherwise returns a
      negative value.
    */
    h5err = H5TBget_field_info (itsFileID,
				itsName.c_str(),
				itsFieldNames,
				NULL,
				NULL,
				NULL);
    if (h5err<0) {
      std::cerr << "[dalTable::h5addColumn_setup]"
		<< " Failed to retrieve information about table field!" 
		<< std::endl;
      return false;
    }

    /*________________________________________________________________
      Check to make sure column doesn't already exist
    */

    for (unsigned int ii=0; ii<nfields; ii++) {
      if (0 == strcmp( column_name.c_str(), itsFieldNames[ii] )) {
	std::cerr << "WARNING: Cannot create column \'"
		  << column_name.c_str()
		  <<	"\'. Column already exists." << endl;
	return false;
      }
      else if (0 == strcmp("000dummy000",itsFieldNames[ii])) {
	removedummy = true;
      }
    }
    
    for (unsigned int ii=0; ii<nfields; ii++) {
      free(itsFieldNames[ii]);
    }
    free(itsFieldNames);

    return true;
  }
  
  //_____________________________________________________________________________
  //                                                           h5addColumn_insert

  /*!
    \param indims       -- Numer of dimensions
    \param colname      -- Name of the table column
    \param field_type   -- 
    \param removedummay -- 
  */
  void dalTable::h5addColumn_insert (uint const & indims,
                                     std::string const & colname,
                                     hid_t const & field_type,
                                     bool const & removedummy )
  {
    // set additional required fields for new column call
    int * data = NULL;
    data       = new int[indims];

    for (unsigned int idx=0; idx<indims; idx++) {
      data[idx] = 0;
    }

    // create the new column
    status = H5TBinsert_field (itsFileID,
			       itsName.c_str(),
			       colname.c_str(),
                               field_type,
			       nfields,
			       data,
			       data);

    delete [] data;
    data = NULL;

    if ( removedummy ) {
      removeColumn("000dummy000");
    }
  }

  //_____________________________________________________________________________
  //                                                                    addColumn
  
  /*!
    \param colname Name of the column you want to add.
    \param coltype Datatype of the column you want to add (i.e. dalINT,
           dalFLOAT, dalSTRING, etc.)
    \param dims Number of dimensions of the column you want to add.
  */
  void dalTable::addColumn( std::string colname,
                            std::string coltype,
                            uint indims )
  {
    if (itsFiletype.type()==dalFileType::HDF5) {
      bool removedummy = false;
      
      h5addColumn_setup( colname, removedummy );
      
      // set the column type
      hsize_t dd = indims;
      hsize_t * dims = &dd;
      hid_t	field_type = 0;
      hid_t   h5type = 0;
      
      if ( dal_CHAR == coltype )
	h5type =  H5T_NATIVE_CHAR;
      
      else if ( dal_INT == coltype )
	h5type =  H5T_NATIVE_INT;
      
      else if ( dal_UINT == coltype )
	h5type = H5T_NATIVE_UINT;
      
      else if ( dal_SHORT == coltype )
	h5type = H5T_NATIVE_SHORT;
      
      else if ( dal_FLOAT == coltype )
	h5type = H5T_NATIVE_FLOAT;
      
      else if ( dal_DOUBLE == coltype )
	h5type = H5T_NATIVE_DOUBLE;
      
      else if ( dal_STRING == coltype )
	h5type = H5T_NATIVE_CHAR;
      
      else if ( dal_COMPLEX_CHAR == coltype || dal_COMPLEX == coltype ||
		dal_COMPLEX_SHORT == coltype )
	{
	  std::vector<dalColumn> cv;
	  std::string component_type;
	  
	  if ( dal_COMPLEX_CHAR == coltype )
	    component_type = dal_CHAR;
	  
	  else if ( dal_COMPLEX == coltype )
	    component_type = dal_DOUBLE;
	  
	  else if ( dal_COMPLEX_SHORT == coltype )
	    component_type = dal_SHORT;
	  
	  dalColumn col_a( "r", component_type );  // real component
	  dalColumn col_b( "i", component_type );  // imaginary component
	  
	  cv.push_back( col_a );
	  cv.push_back( col_b );
	  
	  size_t sz = 0;
	  
	  // compute the size of the compound column
	  for ( unsigned int ii=0; ii<cv.size(); ii++) {
	    sz += cv[ii].getSize();
	  }
	  
	  // create a compound type that can hold each field
	  h5type = H5Tcreate( H5T_COMPOUND, sz );
	  
	  size_t offset = 0;
	  hid_t lcl_datatype = 0;
	  for ( unsigned int ii=0; ii<cv.size(); ii++)
	    {
	      
	      if (0==ii) {
		offset=0;
	      } else {
		offset += cv[ii-1].getSize();
	      }

	      if ( dal_CHAR == cv[ii].getType() )
		lcl_datatype = H5T_NATIVE_CHAR;
	      
	      else if ( dal_SHORT == cv[ii].getType() )
		lcl_datatype = H5T_NATIVE_SHORT;
	      
	      else if ( dal_INT == cv[ii].getType() )
		lcl_datatype = H5T_NATIVE_INT;
	      
	      else if ( dal_FLOAT == cv[ii].getType() )
		lcl_datatype = H5T_NATIVE_FLOAT;
	      
	      else if ( dal_DOUBLE == cv[ii].getType() )
		lcl_datatype = H5T_NATIVE_DOUBLE;
	      
	      H5Tinsert( h5type, cv[ii].name().c_str(), offset, lcl_datatype );
	    }
	}
      else {
	std::cerr << "ERROR: column type " << coltype << " is not supported."
		  << endl;
	return;
      }
      if ((*dims) > 1) {
	field_type = H5Tarray_create( h5type, 1, dims);
      } else {
	field_type = h5type;
      }
      
      h5addColumn_insert( indims, colname, field_type, removedummy );
    }
    else {
      std::cerr << "Operation not yet supported for type " << itsFiletype.name()
		<< ".  Sorry.\n";
    }
  }
  
  //_____________________________________________________________________________
  //                                                             addComplexColumn
  
  /*!
    Create an complex type column to the table.  This is usually called by the
    addColumn method when the addColumn datatype is dalCOMPLEX, and not by the
    developer.

    \param compname Name of the column you want to add.
    \param ri The real and imaginary column components, as represented by
           dalColumn objects.
    \param subfields This is the dimensionality of the complex column type
           and should always be 2.
  */
  void dalTable::addComplexColumn( std::string compname,
                                   std::vector<dalColumn> foo,
                                   int subfields )
  {
    if (itsFiletype.type()==dalFileType::HDF5)
      {
        bool removedummy = false;

        h5addColumn_setup( compname, removedummy );

        // ----------   begin complex column-specific code. -------------

        size_t sz = 0;

        // compute the size of the compound column
        for ( unsigned int ii=0; ii< foo.size(); ii++ )
          {
// 		cerr << "subcolumn name is " << foo[ii].getName()
// 			 << ". Type is " << foo[ii].getType() << endl;
            sz += foo[ii].getSize();
          }

        // create a compound type that can hold each field
        hid_t fieldtype = H5Tcreate( H5T_COMPOUND, sz );

        size_t offset = 0;
        for ( unsigned int ii=0; ii<foo.size(); ii++)
          {

            if (0==ii)
              offset=0;
            else
              offset += foo[ii-1].getSize();

            if ( dal_CHAR == foo[ii].getType() )
              {
                H5Tinsert( fieldtype, foo[ii].name().c_str(), offset,
                           H5T_NATIVE_CHAR);
              }
            else if ( dal_SHORT == foo[ii].getType() )
              {
                H5Tinsert( fieldtype, foo[ii].name().c_str(), offset,
                           H5T_NATIVE_SHORT);
              }
            else if ( dal_INT == foo[ii].getType() )
              {
                H5Tinsert( fieldtype, foo[ii].name().c_str(), offset,
                           H5T_NATIVE_INT);
              }
            else if ( dal_FLOAT == foo[ii].getType() ) {
	      H5Tinsert( fieldtype, foo[ii].name().c_str(), offset,
			 H5T_NATIVE_FLOAT);
	    }
            else if ( dal_DOUBLE == foo[ii].getType() ) {
	      H5Tinsert( fieldtype, foo[ii].name().c_str(), offset,
			 H5T_NATIVE_DOUBLE);
	    }
          }
	
	// ----------   end complex column-specific code. -------------

        h5addColumn_insert( subfields, compname, fieldtype, removedummy );

        return;

        // if successful, add corresponding column object to list
        if ( 0 == status )
          {
            dalColumn * lc = new dalColumn( compname, "foo" );

            // add new dalColumn to the 'columns' vector
            columns.push_back( *lc );

            lc->addMember( "a_member", dal_INT );
          }
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << itsFiletype.name()
		  << ".  Sorry.\n";
      }

  }

  //_____________________________________________________________________________
  //                                                                 removeColumn
  
  /*!
    \param colname Name of the column you want to remove from the table.
  */
  void dalTable::removeColumn (const std::string &colname)
  {
    if (itsFiletype.type()==dalFileType::HDF5)
      {
        status = H5TBget_table_info (itsFileID,
				     itsName.c_str(),
				     &nfields,
				     &nofRecords_p);
        if ( nfields < 2 )
          {
            std::cerr << "WARNING: Cannot delete last column." << endl;
            return;
          }


        hsize_t nfields_start = nfields;

        itsFieldNames = (char **)malloc(nfields_start * sizeof(char *));
        for (unsigned int ii=0; ii<nfields_start; ii++)
          {
            itsFieldNames[ii] = (char *)malloc(MAX_COL_NAME_SIZE * sizeof(char));
          }

        status = H5TBget_field_info (itsFileID,
				     itsName.c_str(),
				     itsFieldNames,
				     NULL,
                                     NULL,
				     NULL);

        bool columnpresent = false;
        for (unsigned int ii=0; ii < nfields; ii++) {
	  
	  if (0 == strcmp(colname.c_str(),itsFieldNames[ii])) {
	    
	    status = H5TBdelete_field( itsFileID, itsName.c_str(),
				       itsFieldNames[ii]);
	    
	    status = H5TBget_table_info (itsFileID,
					 itsName.c_str(),
					 &nfields,
					 &nofRecords_p);
	    columnpresent = true;
	    break;
	    
	  }
	}
	
        for (unsigned int ii=0; ii<nfields_start; ii++) {
	  free(itsFieldNames[ii]);
	}
        free(itsFieldNames);
	
        if ( !columnpresent )
          std::cerr << "WARNING: Column \'" << colname <<
	    "\' not present.  Cannot delete." << endl;
      }
    else {
      std::cerr << "Operation not yet supported for type " << itsFiletype.name()
		<< ".  Sorry.\n";
    }
  }
  
  // ----------------------------------------------------- writeDataByColNum
  
  /*!
    \brief Write data to a column identified by it's position.
    
    Write data to a column identified by it's position within the table.
    
    \param data A pointer to a structure containing the data you want
    to write.
    \param index The position of the column you want to write to.
    \param rownum The row position where you want to start writing data.
  */
  void dalTable::writeDataByColNum (void * data,
				    int index,
				    int rownum,
				    long nrecs)
  {
    if (itsFiletype.type()==dalFileType::HDF5)
      {
	
        size_t * field_sizes = NULL;
        size_t * field_offsets = NULL;
        size_t * size_out = NULL;
	
        // retrieve the input fields needed for the append_records call
        H5TBget_table_info ( itsFileID, itsName.c_str(), &nfields, &nofRecords_p );
	
        field_sizes   = new size_t[ nfields ];
        field_offsets = new size_t[ nfields ];
        size_out      = new size_t[ 1 ];

        status = H5TBget_field_info( itsFileID, itsName.c_str(), NULL, field_sizes,
                                     field_offsets, size_out );
        /*
         * Cleanup to make more efficient.  Check the last three fields for
         * H5TBget_field_info();
         */
        int num_fields 		= 1;	  // number of fields to overwrite
        const int inum		= index;  // column number to overwrite
        const int * index_num	= &inum;  // pointer to column number to overwrite
        hsize_t start		= hsize_t(rownum); // record index to start at
        hsize_t numrecords	= nrecs;	  // number of records to write

        size_t col_offset[1] = { 0 };
        size_t col_size[1] = { field_sizes[index] };
        status = H5TBwrite_fields_index(itsFileID, itsName.c_str(), num_fields,
                                        index_num, start, numrecords, *col_size,
                                        col_offset, col_size, data);

        delete [] field_sizes;
        field_sizes = NULL;
        delete [] field_offsets;
        field_offsets = NULL;
        delete [] size_out;
        size_out = NULL;

      }
    else {
      std::cerr << "Operation not yet supported for type " << itsFiletype.name()
		<< ".  Sorry.\n";
    }
  }
  
  //_____________________________________________________________________________
  //                                                                    appendRow
  
  /*!
    \brief Append a row to the table.

    Append a row to the table.

  \param data The data you want to write at the end of the table.  The
  		  structure of the data parameter should match that of the
  		  table itself.
  */
  void dalTable::appendRow( void * data )
  {
    if (itsFiletype.type()==dalFileType::HDF5)
      {
        hsize_t recs2write     = 1; // number of records to append
        size_t * field_sizes   = NULL;
        size_t * field_offsets = NULL;
        size_t * size_out      = NULL;

        // retrieve the input fields needed for the append_records call
        H5TBget_table_info ( itsFileID, itsName.c_str(), &nfields, &nofRecords_p );

        field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
        field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
        size_out = (size_t*)malloc( sizeof(size_t) );

        status = H5TBget_field_info( itsFileID, itsName.c_str(), NULL, field_sizes,
                                     field_offsets, size_out );

        if ( itsFirstRecord )
          {
            hsize_t start = 0;
            hsize_t numrows = 1;
            status = H5TBwrite_records( itsFileID, itsName.c_str(), start,
                                        numrows, *size_out, field_offsets, field_sizes,
                                        data);
            itsFirstRecord = false;
          }
        else
          {
            status = H5TBappend_records ( itsFileID, itsName.c_str(), recs2write,
                                          *size_out, field_offsets,
                                          field_sizes, data );
          }
        free( field_sizes );
        free( field_offsets );
        free( size_out );
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << itsFiletype.name()
		  << ".  Sorry.\n";
      }
  }

  //_____________________________________________________________________________
  //                                                                   appendRows
  
  /*!
    \brief Append multiple rows.

    Append multiple rows to the end of the table.

    \param data The data you want to write at the end of the table.  The
                structure of the data parameter should match that of the
                table itself.
    \param row_count The number of rows you wish to append.
  */
  void dalTable::appendRows (void * data,
			     long row_count)
  {
    if (itsFiletype.type()==dalFileType::HDF5)
      {
        size_t * field_sizes = NULL;
        size_t * field_offsets = NULL;
        size_t * size_out = NULL;

        // retrieve the input fields needed for the append_records call
        H5TBget_table_info( itsFileID, itsName.c_str(), &nfields, &nofRecords_p );

        field_sizes  = (size_t *)malloc((size_t)nfields * sizeof(size_t));
        field_offsets = (size_t *)malloc((size_t)nfields * sizeof(size_t));
        size_out = (size_t*)malloc( sizeof(size_t) );

        status = H5TBget_field_info( itsFileID, itsName.c_str(), NULL, field_sizes,
                                     field_offsets, size_out );

        if ( itsFirstRecord )
          {
            hsize_t start = 0;
            if (row_count>1)
              {
                status = H5TBappend_records ( itsFileID, itsName.c_str(),
                                              (hsize_t)row_count-1, *size_out,
                                              field_offsets, field_sizes, data);
              }
            status = H5TBwrite_records( itsFileID, itsName.c_str(), start,
                                        (hsize_t)row_count, *size_out,
                                        field_offsets, field_sizes, data );
            itsFirstRecord = false;
          }
        else
          {
            status = H5TBappend_records ( itsFileID, itsName.c_str(),
                                          (hsize_t)row_count, *size_out,
                                          field_offsets, field_sizes, data);
          }
        free( field_sizes );
        free( field_offsets );
        free( size_out );
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << itsFiletype.name()
		  << ".  Sorry.\n";
      }
  }

  //_____________________________________________________________________________
  //                                                                 setAttribute
  
  /*!
    \brief Define a string attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalTable::setAttribute( std::string attrname,
                               std::string data )
  {
    return HDF5Attribute::write ( itsTableID, attrname, &data, 1 );
  }

  //_____________________________________________________________________________
  //                                                                  listColumns
  
  /*!
    \brief List the table columns.

    \return a list of columns contained within the opened table.
   */
  std::vector<std::string> dalTable::listColumns()
  {
    std::vector<std::string> colnames;
    colnames.clear();
    
    if (itsFiletype.type()==dalFileType::HDF5) {
      size_t * field_sizes = NULL;
      size_t * field_offsets = NULL;
      size_t * size_out = NULL;
      
      // retrieve the input fields needed for the append_records call
      H5TBget_table_info ( itsFileID, itsName.c_str(), &nfields, &nofRecords_p );
      
      field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
      field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
      size_out = (size_t*)malloc( sizeof(size_t) );
      
      /* Alocate space */
      itsFieldNames = (char**)malloc( sizeof(char*) * (size_t)nfields );
      for ( unsigned int i = 0; i < nfields; i++)
	{
	  itsFieldNames[i]=(char*)malloc(sizeof(char) * MAX_COL_NAME_SIZE );
	}
      status = H5TBget_field_info( itsFileID, itsName.c_str(), itsFieldNames,
				   field_sizes, field_offsets, size_out );
      
      for (unsigned int ii=0; ii<nfields; ii++)
	{
	  colnames.push_back( itsFieldNames[ii] );
	  free(itsFieldNames[ii]);
	}
      free(itsFieldNames);
      return colnames;
    }
    else if (itsFiletype.type()==dalFileType::CASA_MS)
      {
#ifdef DAL_WITH_CASA
        casa::TableDesc td = itsCasaTable->tableDesc();
        casa::Vector<casa::String> names = td.columnNames();
        casa::Array<casa::String> myarray = names;
        std::vector<casa::String> foovec;
        myarray.tovector( foovec );
        for ( uint idx=0; idx < foovec.size(); idx++ )
          colnames.push_back( std::string( foovec[ idx ].c_str() ) );
        return colnames;
#else
        std::cerr << "ERROR: CASA support not enabled.\n";
#endif
      }
    else {
      std::cerr << "Operation not yet supported for type " << itsFiletype.name()
		<< ".  Sorry.\n";
      return colnames;
    }
    
    return colnames;
  }
  
  //_____________________________________________________________________________
  //                                                              getNumberOfRows
  
  /*!
    \brief Get the number of rows in a table.
    
    \return nofRows -- The number of rows in the table as a long precision
            integer.
  */
  long dalTable::getNumberOfRows()
  {

    switch (itsFiletype.type()) {
    case dalFileType::HDF5:
      {
	if (itsFileID > 0) {
	  H5TBget_table_info (itsFileID,
			      itsName.c_str(),
			      &nfields,
			      &nofRecords_p);
	}
	else if (itsTableID > 0) {
	  H5TBget_table_info (itsTableID,
			      itsName.c_str(),
			      &nfields,
			      &nofRecords_p);
	}
	return nofRecords_p;
      }
      break;
    default:
      {
	std::cerr << "[dalTable::getNumberOfRows]"
		  << " Operation not yet supported for type "
		  << itsFiletype.name()
		  << std::endl;
	return(-1);
      }
      break;
    };

  }
  
  //_____________________________________________________________________________
  //                                                                     readRows
  
  /*!
    \brief Read rows from the table.
    
    \param data_out A structure which the data will be read in to.  It should
           match the structure of the table itself.
    \param start The row number to start reading from.
    \param stop The row number where you want to stop reading.
    \param buffersize An optional parameter specifying the size of the output
           buffer.
  */
  void dalTable::readRows( void * data_out,
                           long nstart,
                           long numberRecs,
                           long buffersize )
  {
    if (itsFiletype.type()==dalFileType::HDF5)
      {
        // retrieve the input fields needed for the append_records call
        H5TBget_table_info ( itsFileID, itsName.c_str(), &nfields, &nofRecords_p );

        size_t * field_sizes   = new size_t[nfields];
        size_t * field_offsets = new size_t[nfields];
        size_t * size_out      = new size_t[1];
        itsFieldNames = (char**)malloc( nfields * sizeof(char*) );
 
	for (unsigned int ii=0; ii<nfields; ii++) {
	  itsFieldNames[ii] = (char*)malloc(MAX_COL_NAME_SIZE*sizeof(char));
	}
	
        status = H5TBget_field_info (itsFileID,
				     itsName.c_str(),
				     itsFieldNames,
                                     field_sizes,
				     field_offsets,
				     size_out);

        hsize_t start = nstart;
        hsize_t nrecs = numberRecs;

        if (buffersize > 0)
          size_out[0] = buffersize;
        status = H5TBread_records (itsFileID,
				   itsName.c_str(),
				   start,
				   nrecs,
                                   size_out[0],
				   field_offsets,
				   field_sizes,
                                   data_out);
	/* Release allocated memory */
        delete [] field_sizes;
        delete [] field_offsets;
        delete [] size_out;

        for (unsigned int ii=0; ii<nfields; ii++) {
	  free(itsFieldNames[ii]);
	}
        free(itsFieldNames);

        if (status < 0) {
	  std::cerr << "[dalTable::readRows]"
		    << " Problem reading records. Row buffer may be too big."
		    << " Make sure the buffer is smaller than the size of the table."
		    << std::endl;
	}
      }
    else {
      std::cerr << "Operation not yet supported for type "
		<< itsFiletype.name()
		<< ". Sorry."
		<< std::endl;
    }
  }

#ifdef DAL_WITH_CASA

  //_____________________________________________________________________________
  //                                                                findAttribute
  
  /*!
    \param attrname       -- The name of the attribute you want to find.
    \return haveAttribute -- \e True if found. \e False if not found.
  */
  casa::Bool dalTable::findAttribute( std::string attrname )
  {
    if (itsFiletype.type()==dalFileType::HDF5) {
      if ( H5Aexists( itsTableID, attrname.c_str() ) <= 0 ) {
	std::cerr << "Attribute " << attrname << " not found." << endl;
	return false;
      }
      else {
	return true;
      }
    }
    else {
      std::cerr << "Operation not yet supported for type " << itsFiletype.name()
		<< ".  Sorry.\n";
      return false;
    }
  }
#endif
  
  //_____________________________________________________________________________
  //                                                                 getAttribute
  
  /*!
    \brief Get the value of an attribute.
    
    \param attrname The name of the attribute you want to read.
    \return A pointer to the attribute data.
  */
  void * dalTable::getAttribute (std::string attrname )
  {
    
    if (itsFiletype.type()==dalFileType::HDF5)
      {
        hsize_t dims;
        H5T_class_t type_class;
        size_t type_size;
	
        // Check if attribute exists
        if ( H5Aexists( itsTableID, attrname.c_str() ) <= 0 )
          {
            return NULL;
          }

        std::string fullname = "/" + itsName;

        int rank;
        H5LTget_attribute_ndims( itsFileID, fullname.c_str(),
                                 attrname.c_str(), &rank );

        H5LTget_attribute_info( itsFileID, fullname.c_str(), attrname.c_str(),
                                &dims, &type_class, &type_size );

        if ( H5T_FLOAT == type_class )
          {
            void * data = NULL;
            if ( 0 < H5LTget_attribute (itsFileID,
					fullname.c_str(),
					attrname.c_str(),
					H5T_NATIVE_DOUBLE,
					data) )
              return NULL;
            else
              return reinterpret_cast<double*>(data);
          }
        else if ( H5T_INTEGER == type_class )
          {
            void * data = NULL;
            if ( 0 < H5LTget_attribute(itsFileID, fullname.c_str(),
                                       attrname.c_str(),
                                       H5T_NATIVE_INT, data) )
              return NULL;
            else
              return reinterpret_cast<int*>(data);
          }
        else if ( H5T_STRING == type_class )
          {
            char* data = NULL;
            std::string fullname = "/" + itsName;
            data = (char *)malloc(rank * sizeof(char));
            if ( 0 < H5LTget_attribute_string( itsFileID,
					       fullname.c_str(),
                                               attrname.c_str(),
					       data))
              return NULL;
            else
              return data;
          }
        else
          {
            return NULL;
          }
      }
    else if (itsFiletype.type()==dalFileType::CASA_MS)
      {
#ifdef DAL_WITH_CASA
        casa::TableRecord table_rec = itsCasaTable->keywordSet();
        casa::String dtype = GetKeywordType(attrname);
        if (dtype == "Float")
          {
            casa::Float result;
            casa::Float * result_p = &result;
            if (GetKeyword(attrname,&result))
              return result_p;
            else
              {
                std::cerr << "ERROR: dalTable:GetKeyword Could not get '"
			  << attrname
			  << endl;
                return NULL;
              }
          }
        else
          {
            std::cerr << "(TBD) datatype is " << itsFiletype.name() << endl;
            return NULL;
          }
#else
        std::cerr << "ERROR: CASA not enabled [dalTable::getAttribute()]" << endl;
        return NULL;
#endif
      }
    else
      {
        std::cerr << "Operation not yet supported for type " << itsFiletype.name()
		  << ".  Sorry.\n";
        return NULL;
      }

    return NULL;
  }

#ifdef DAL_WITH_CASA

  //_____________________________________________________________________________
  //                                                                   GetKeyword
  
  /*!
    \brief Retrieve a casa keykword.

    Retrieve a casa keykword.

    \param KeywordName Name of the keyword to retrieve.
    \param result Keyword value.
    \return Casa boolean value. True on success. False on fail.
   */

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName,
                                  casa::String *result)
  {
#ifdef DAL_DEBUGGING_MESSAGES
    std::cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
#endif
    try
      {
        if (!itsCasaTable->keywordSet().isDefined(KeywordName))
          {
            std::cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName <<
                      "\" does not exist" << endl;
            return casa::False;
          };
        *result = itsCasaTable->keywordSet().asString(KeywordName);
      }
    catch (casa::AipsError x)
      {
        std::cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
        return casa::False;
      };
    return casa::True;
  }

  //_____________________________________________________________________________
  //                                                                   GetKeyword
  
  /*!
    \brief Retrieve a casa keykword.

    \param KeywordName Name of the keyword to retrieve.
    \param result Keyword value.
    \return Casa boolean value. True on success. False on fail.
   */

  casa::Bool dalTable::GetKeyword (casa::String const KeywordName,
				   casa::Double *result)
  {
#ifdef DAL_DEBUGGING_MESSAGES
    std::cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
#endif
    try {
      if (!itsCasaTable->keywordSet().isDefined(KeywordName))
	{
	  std::cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName <<
	    "\" does not exist" << endl;
	  return casa::False;
	};
      *result = itsCasaTable->keywordSet().asDouble(KeywordName);
    }
    catch (casa::AipsError x) {
      std::cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
      return casa::False;
    };
    return casa::True;
  }
  
  //_____________________________________________________________________________
  //                                                                   GetKeyword
  
  /*!
    \brief Retrieve a casa keykword.

    \param KeywordName Name of the keyword to retrieve.
    \param result Keyword value.
    \return Casa boolean value. True on success. False on fail.
   */

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName,
                                  casa::Float *result)
  {
#ifdef DAL_DEBUGGING_MESSAGES
    std::cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
#endif
    try
      {
        if (!itsCasaTable->keywordSet().isDefined(KeywordName))
          {
            std::cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName <<
                      "\" does not exist" << endl;
            return casa::False;
          };
        *result = itsCasaTable->keywordSet().asFloat(KeywordName);
      }
    catch (casa::AipsError x)
      {
        std::cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
        return casa::False;
      };
    return casa::True;
  }

  //_____________________________________________________________________________
  //                                                                   GetKeyword
  
  /*!
    \brief Retrieve a casa keykword.

    \param KeywordName Name of the keyword to retrieve.
    \param result Keyword value.
    \return Casa boolean value. True on success. False on fail.
   */

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName,
                                  casa::DComplex *result)
  {
#ifdef DAL_DEBUGGING_MESSAGES
    std::cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
#endif
    try
      {
        if (!itsCasaTable->keywordSet().isDefined(KeywordName))
          {
            std::cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName <<
                      "\" does not exist" << endl;
            return casa::False;
          };
        *result = itsCasaTable->keywordSet().asDComplex(KeywordName);
      }
    catch (casa::AipsError x)
      {
        std::cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
        return casa::False;
      };
    return casa::True;
  }

  //_____________________________________________________________________________
  //                                                                   GetKeyword
  
  /*!
    \brief Retrieve a casa keykword.

    \param KeywordName Name of the keyword to retrieve.
    \param result Keyword value.
    \return Casa boolean value. True on success. False on fail.
   */

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName,
                                  casa::Array<casa::Double> *result)
  {
#ifdef DAL_DEBUGGING_MESSAGES
    std::cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
#endif
    try
      {
        if (!itsCasaTable->keywordSet().isDefined(KeywordName))
          {
            std::cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName <<
                      "\" does not exist" << endl;
            return casa::False;
          };
        result->reference(itsCasaTable->keywordSet().asArrayDouble(KeywordName));
      }
    catch (casa::AipsError x)
      {
        std::cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
        return casa::False;
      };
    return casa::True;
  }

  //_____________________________________________________________________________
  //                                                                   GetKeyword
  
  /*!
    \brief Retrieve a casa keykword.

    \param KeywordName Name of the keyword to retrieve.
    \param result Keyword value.
    \return Casa boolean value. True on success. False on fail.
   */

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName,
                                  casa::Array<casa::DComplex> *result)
  {
#ifdef DAL_DEBUGGING_MESSAGES
    std::cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
#endif
    try
      {
        if (!itsCasaTable->keywordSet().isDefined(KeywordName))
          {
            std::cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName <<
                      "\" does not exist" << endl;
            return casa::False;
          };
        result->reference(itsCasaTable->keywordSet().asArrayDComplex(KeywordName));
      }
    catch (casa::AipsError x)
      {
        std::cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
        return casa::False;
      };
    return casa::True;
  }


  //_____________________________________________________________________________
  //                                                               GetKeywordType
  
  /*!
    \brief Get the type of the given casa keyword.

    \param KeywordName Name of the keyword to retrieve.
    \return String containing the keyword type.
   */

  casa::String dalTable::GetKeywordType (casa::String const KeywordName)
  {
    try {
      if (!itsCasaTable->keywordSet().isDefined(KeywordName))
	{
	  std::cerr << "dalTable::GetKeywordType: Keyword named \"" << KeywordName <<
	    "\" does not exist" << endl;
	  return "";
	};
      casa::DataType type = itsCasaTable->keywordSet().dataType(KeywordName);
      switch (type)
	{
	case casa::TpString:
	  return "String";
	case casa::TpFloat:
	  return "Float";
	case casa::TpDouble:
	  return "Double";
	case casa::TpDComplex:
	  return "DComplex";
	case casa::TpArrayDouble:
	  return "Array<Double>";
	case casa::TpArrayDComplex:
	  return "Array<DComplex>";
	case casa::TpInt:
	  return "Int";
	case casa::TpUInt:
	  return "uInt";
	default:
	  return "unknown";
	}
    }
    catch (casa::AipsError x)
      {
        std::cerr << "dalTable::GetKeywordType: " << x.getMesg() << endl;
        return "unknown";
      };
  }
#endif // DAL_WITH_CASA
  
} // end namespace DAL
