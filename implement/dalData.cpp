/*-------------------------------------------------------------------------*
 | $Id:: dalData.h 660 2007-08-30 16:55:31Z masters                     $ |
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
  \file dalData.h
  
  \ingroup DAL

  \author Joseph Masters

  \date 09-05-07
*/

#ifndef DALDATA_H
#include "dalData.h"
#endif

/*void * dalData::get( long idx1 )
{
  return NULL;
}
void * dalData::get( long idx1 )
{
  return NULL;
}*/
void * dalData::get( long idx1, long idx2, long idx3 )
{
   vector<long> indices;
//    vector<long>::iterator it;
   if( idx1>-1 )
     indices.push_back( idx1 );
   if( idx2>-1 )
     indices.push_back( idx2 );
   if( idx3>-1 )
     indices.push_back( idx3 );

// cout << "ind size: " << indices.size() << endl;
// for (unsigned int dim=0; dim<indices.size(); dim++)
//   cout << "   " << indices[ dim ] << endl;
// cout << "shape size: " << shape.size() << endl;
// for (unsigned int dim=0; dim<shape.size(); dim++)
//   cout << "   " << shape[ dim ] << endl;

   if (indices.size() != (shape.size()) )
   {
      cout << "ERROR: Number of indices do not match shape of column." << endl;
      exit(-1);
   }
   unsigned long index = 0;
   long bb = 1;

   if ( "MSCASA" == filetype )
   {
//      it = shape.begin();
     shape.insert( shape.begin(), 1 );
     for (unsigned int dim=0; dim<shape.size()-1; dim++)
     {
//         index = idx1 + (idx2*shape[0]) + (idx3*(shape[0]*shape[1]));
        for (unsigned int ss=dim; ss>0; ss--)
	   bb *= shape[ ss ];

        index += indices[dim]*bb;
	bb=1;
     }
     shape.erase( shape.begin() );

     if ( dal_COMPLEX == datatype )
        return (&(((complex<float>*)data)[ index ]));

     else if ( dal_DOUBLE == datatype )
        return (&(((double*)data)[ index ]));
   }
   return NULL;
}

dalData::dalData() {}

dalData::dalData(string lclfiletype, string lcldatatype,
   vector<int> lclshape, long lclnrows)
{
    filetype = lclfiletype;
    datatype = lcldatatype;
    shape = lclshape;
    nrows = lclnrows;
}

#ifdef PYTHON
bpl::numeric::array dalData::get_boost()
{
  bpl::list data_list;
  if ( 1 == shape.size() ) // SCALAR
    {
      if ( dal_DOUBLE == datatype )
      {
        for (unsigned int ii=0; ii<nrows; ii++)
        {
           data_list.append( (*((double*)get(ii))) );
        }
        bpl::numeric::array nadata(
            bpl::make_tuple(data_list)
        );
        bpl::list lcl_dims;
        for (unsigned int hh=0; hh<shape.size(); hh++)
        { lcl_dims.append(shape[hh]);}

        nadata.setshape( lcl_dims );
        return nadata;
      }
      else 
      {
	for (int ii=0; ii<1; ii++)
		data_list.append(0);
		bpl::numeric::array nadata( data_list );
	return nadata;
      }
    }
    else  // ARRAY
    {
      if ( dal_DOUBLE == datatype )
      {
        if ( 2 == shape.size() ) // 2D case
        {
  	  for ( int xx=0; xx<shape[0]; xx++)
  	    for ( int yy=0; yy<shape[1]; yy++)
              data_list.append( (*((double*)get(xx,yy))) );
        }
        else if ( 3 == shape.size() ) // 3D case
        {
  	  for ( int xx=0; xx<shape[0]; xx++)
  	    for ( int yy=0; yy<shape[1]; yy++)
  	      for ( int zz=0; zz<shape[2]; zz++)
                data_list.append( (*((double*)get(xx,yy,zz))) );
        }
 
        bpl::numeric::array nadata( data_list );
 
        bpl::list lcl_dims;
        for (unsigned int hh=0; hh<shape.size(); hh++)
          lcl_dims.append(shape[hh]);
 
        nadata.setshape( lcl_dims );
        return nadata;
      }
      else if ( dal_COMPLEX == datatype )
      {
        complex<float> * tmp_comp;
        if ( 2 == shape.size() ) // 2D case
        {
  	  for (int xx=0; xx<shape[0]; xx++)
  	    for (int yy=0; yy<shape[1]; yy++)
              data_list.append( (*((complex<float>*)get(xx,yy))) );
        }
        else if ( 3 == shape.size() ) // 3D case
        {
long count=0;
  	  for (int xx=0; xx<shape[0]; xx++)
  	    for ( int yy=0; yy<shape[1]; yy++)
  	      for (int zz=0; zz<shape[2]; zz++)
              {
//   complex<float> * value3;
//   value3 = (complex<float>*)data_object3->get(2,45,xx);
//   cout << *value3 << endl;
		tmp_comp = (complex<float>*)get(xx,yy,zz);
                data_list.append( *tmp_comp );
	        count++;
              }
// cout << "count " << count << endl;
        }
        bpl::numeric::array nadata( data_list );
 
        bpl::list lcl_dims;
        for (unsigned int hh=0; hh<shape.size(); hh++)
        {  lcl_dims.append(shape[hh]); /*cout << "appending " << shape[hh] << endl;*/ }
// cout << bpl::len(data_list) << endl; 
        nadata.setshape( lcl_dims );
        return nadata;
      }
    }
    for (int ii=0; ii<1; ii++)
	data_list.append(0);
    bpl::numeric::array nadata( data_list );
    return nadata;
}
#endif