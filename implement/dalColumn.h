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

#ifndef DALCOLUMN_H
#define DALCOLUMN_H

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALATTRIBUTE_H
#include "dalAttribute.h"
#endif

#ifndef DALFILTER_H
#include "dalFilter.h"
#endif

/*!
  \class dalColumn
  
  \ingroup DAL
  
  \author Joseph Masters
*/
class dalColumn {

	string name; /// column name
	string datatype;  /// column datatype
	int size; /// datatype size
	int totsize; /// total column size
	vector<dalAttribute> attributes; /// list of column attributes
	dalFilter filter; /// filter associated with column

	// hdf5-specific variables
	hid_t coltype;
	herr_t  status;  /// hdf5 call return status

#ifdef WITH_CASA
	// casa-specific variables
	casa::ROTableColumn * casa_column;
	casa::ColumnDesc casa_col_desc;
	
#endif

public:
	dalColumn();
	dalColumn( string colname, string coltype );
	dalColumn( casa::Table table, string colname );
	dalColumn( string complexcolname );
	void addMember( string member_name, string type );
	string getName();
	string getType();
	int getSize();
	void close();
	void type();
// 	void data();
	int isArray();
	int isScalar();
	void shape();
	void ndims();

//	int getNumber();  /// return the index of a column
// 	void addArray();
//	int insertAfter(); /// insert a new column after column specified by name or number
//	int insertBefore(); /// insert a new column before column specified by name or number
//	int replace(); /// replace a column with a new one
//	int remove();  /// remove a column
};
#endif
