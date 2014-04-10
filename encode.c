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

/* this routine is used to convert an old Node to a new type of node  */
NODE unaryConvert(NODE, oldNode){
      if(oldNode->type = TYFLOAT){
          NODE newNode = malloc(sizeof(struct exprtree_node));
          newNode->exprTypeTag = CONV;
          newNode->type = TYDOUBLE;
          newNode->u.convert.oldType = TYFLOAT;
          newNode->u.convert.newType = TYDOUBLE;
          newNode->u.convert.child = oldNode;
          oldNode = newNode;
       }
      if(oldNode->type == TYSUBRANGE){   //convert a subrange type to its base type
          ST_ID id;
          int temp;
          long low, high;
          if(oldNode->expreTypeTag == VARIABLE){
               id = st_lookup_id(oldNode->u.var_name);
            }
          else if(oldNode->tag == DEREF){
               id = st_lookup_id(oldNode->u.deref.child->u.var_name);
            }
          ST_DR entry = st_lookup(id, &temp);
          TYPE type = ty_query_subrange(entry->u.decl.type, &low, &high);
          TYPETAG newType = ty_query(type);
          
          NODE newNode = malloc(sizeof(struct exprtree_node));
          newNode->exprTypeTag = CONV;
          newNode->type = newType;
          newNode->u.convert.oldType = TYSUBRANGE;
          newNode->u.convert.newType = newType;
          newNode->u.convert.child = oldNode;
          oldNode = newNode;          
       }
      
      return oldNode;
  }


