/*
 * encode.c of Pascal Compiler
 * University of South Carolina
 * author: Yixing Cheng, Zibo Meng, Ruofan Xia
 * date:4/09/2014
 *
*/

#include "encode.h"

void calSizeAlign(TYPE ty, int *align, unsigned int *size)
{
   TYPETAG tytag = ty_query(ty);
   if(tytag == TYSIGNEDLONGINT)
   {
      *size = 4;
      *align = 4;
   }
   else if(tytag == TYSIGNEDINT)
   {
      *size = 4;
      *align = 4;
   }
   else if(tytag == TYPTR)
   {
      *size = 4;
      *align = 4;
   }
   else if(tytag == TYFLOAT)
   {
      *size = 4;
      *align = 4;
   }
   else if(tytag == TYDOUBLE)
   {
      *size = 8;
      *align = 8;
   }
   else if(tytag == TYUNSIGNEDCHAR)
   {
      *size = 1;
      *align = 1;
   }
   else if(tytag == TYSIGNEDCHAR)
   {
      *size = 1;
      *align = 1;
   }
   else if(tytag == TYARRAY)
   {
      INDEX_LIST ilist;
      long low,high;
      TYPE tyarr = ty_query_array(ty,&ilist);
      calSizeAlign(tyarr,align,size);
      while(ilist)
      { 
         TYPE tyrng = ty_query_subrange(ilist->type,&low,&high);
         long range = high - low + 1;
         *size = *size * range;
         ilist = ilist->next;
      }
      
   }
   else if(tytag == TYSUBRANGE)
   {
      long low,high;
      calSizeAlign(ty_query_subrange(ty,&low,&high),align,size);
   }
}
