/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

/*!
  \file dalTable.h
  
  \ingroup DAL

  \author Joseph Masters

  \date 12-04-06
*/

#ifndef DALTABLE_H
#define DALTABLE_H

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALATTRIBUTE_H
#include "dalAttribute.h"
#endif

#ifndef DALFILTER_H
#include "dalFilter.h"
#endif

#ifndef DALCOLUMN_H
#include "dalColumn.h"
#endif

/*!
A dalTable can reside within a dataset, or within a group that is within
a dataset.
*/
class dalTable{

	void * file;		/// can be HDF5File, FITS, MS

	// HDF5-specific variables
	hid_t file_id; 	/// hdf5 file_id
	hid_t table_id;	/// hdf5 table id
	hsize_t nfields; /// hdf5 field count
	hsize_t nrecords; /// hdf5 record count
	herr_t status; /// hdf5 return status
	char **field_names;  /// hdf5 list of columns

	bool firstrecord;
	string name; /// table name
	string type;  /// "HDF5", "MSCASA" or "FITS"; for example
	vector<dalAttribute> attributes; /// list of table attributes
	dalFilter filter; /// filter associated with table
	vector<dalColumn> columns; /// list of table columns

	long nrows; /// number of table rows
	int ncols; /// number of table columns

#ifdef WITH_CASA
	// casa-specific variables
	casa::Table * casa_table_handle;
	casa::Array<casa::Double> array_vals_dbl;
	casa::Array<casa::Complex> array_vals_comp;
#endif

  public:

	dalTable();
	dalTable( string filetype ); /// Constructor
	~dalTable(); /// Destructor

 	void getAttributes();
	void printColumns();  /// list the columns contained within a table
	void openTable( void * voidfile, string tablename, string groupname );
#ifdef WITH_CASA
	void openTable( string tablename );
	void openTable( /*void * voidfile,*/ string tablename,
	  casa::MSReader * reader );
	void openTable( /*void * voidfile,*/ string tablename,
	  casa::MSReader * reader, /*string parse_string*/dalFilter * filter );
#endif
	void createTable( void * voidfile, string tablename, string groupname );
	dalColumn * getColumn( string colname );
	void addColumn( string colname, string coltype, int size=1 );
	void addArrayColumn( string colname, string coltype, unsigned int dims);
	void addComplexColumn( string compname, vector<dalColumn>,
				int subfields );
	void removeColumn( string colname );
	void writeDataByColNum( void * structure, int index, int rownum );
	
// 	void writeDataByColName( void * structure, string colname );
	void appendRow( void * data );
	void appendRows( void * data, long number_of_rows );
	void listColumns( /*void * data_out, long start, long stop*/ );
	void readRows( void * data_out, long start, long stop, long buffersize=0 );
	void * getAttribute( string attrname );
	void printAttribute( string attrname );
	//void setAttribute( string attrname, void * data, int size, string datatype );
	void setAttribute_string( string attrname, string data );
	void setAttribute_char( string attrname, char * data, int size=1 );
	void setAttribute_int( string attrname, int * data, int size=1 );
	void setAttribute_uint( string attrname, unsigned int * data, int size=1 );
	void setAttribute_double( string attrname, double * data, int size=1 );
	bool findAttribute( string attrname );
//	int getNumberOfColumns();  /// return the number of columns in a table
	long getNumberOfRows();  /// return the number of rows within a table
//	int setRowNumber();  /// advance to a row by its index
//	int advanceRow();  /// advance one row
	void getName();  /// return the name of the table
//	int setName();  /// set the name of the table
//	int insertRow();  /// insert a row at current location
//	int removeRow();  /// remove a row at current location
	/// retrieve a dalColumn by name
	void * getColumnData( string colname );
//	int getColumnNumber();  /// retrieve a dalColumn by index
//	int listAttributes(); /// return a list of group attributes
//	int getAttribute();  /// return a dalAttribute object retrieved by name
//	int size(); /// return number of table rows
//	int writeColVal(); /// write a value or set of
/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/
#ifdef PYTHON
	void ot_hdf5( void * voidfile, string tablename, string groupname );
	void append_row_boost( bpl::object data );

#ifdef WITH_CASA
	void ot_nonMStable( string tablename );
//	void ot_ms1( string tablename, casa::MSReader * reader);
/*	void ot_ms2( string tablename, casa::MSReader * reader,
	  string parse_string );*/
// 	bpl::numeric::array gcd_boost( string arrayname );
#endif

#endif
};

#endif
