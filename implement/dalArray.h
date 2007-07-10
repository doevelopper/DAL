/*-------------------------------------------------------------------------*
 | $Id:: dalDataset.h 389 2007-06-13 08:47:09Z baehren                   $ |
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

#ifndef dalArray_H
#define dalArray_H

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALATTRIBUTE_H
#include "dalAttribute.h"
#endif

/*!
  \class dalIntArray
  
  \ingroup DAL

  \author Joseph Masters

  \date 07-02-07

  The dalIntArray object holds an n-dimensional array of a single datatype.
*/

class dalArray{

	int rank; // number of dimensions
	string datatype;
	string name;
};

class dalIntArray: public dalArray {
  
  public:
	dalIntArray( void* voidfile, string arrayname, vector<int> dims, int data[]);
	~dalIntArray();
};

class dalFloatArray: public dalArray {
  
  public:
	dalFloatArray( void* voidfile, string arrayname, vector<int> dims, float data[]);
	~dalFloatArray();
};

#endif
