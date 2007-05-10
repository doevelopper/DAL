%module dal
%include "std_string.i"
%include "cpointer.i"

%{
/* Includes the header in the wrapper code */
#include "uvw.h"
#include "cs1.h"
#include "lffe.h"
#include "dal.h"
//#include "lffe_buf.h"
#include "dalAttribute.h"
#include "dalColumn.h"
#include "dalFilter.h"
#include "dalGroup.h"
#include "dalTable.h"
#include "dalDataset.h"
%}

/* Parse the header file to generate wrappers */
%include "uvw.h"
%include "cs1.h"
%include "lffe.h"
%include "dal.h"
//%include "lffe_buf.h"
%include "dalAttribute.h"
%include "dalColumn.h"
%include "dalFilter.h"
%include "dalGroup.h"
%include "dalTable.h"
%include "dalDataset.h"

/* Wrap a class interface around an "int *" */
/* %pointer_class(void, voidp); */
%pointer_class(char, charp);
%pointer_class(short, shortp);
%pointer_class(int, intp);
%pointer_class(double, doublep);
%pointer_class(float, floatp);
%pointer_class(UVW, uvwp);
%pointer_class(CS1, cs1p);
%pointer_class(LFFE, lffep);
//%pointer_class(LFFE_BUF, lffebufp);
