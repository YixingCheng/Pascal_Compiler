/*
 * encode.c of Pascal Compiler
 * University of South Carolina
 * author: Yixing Cheng, Zibo Meng, Ruofan Xia
 * date:4/09/2014
 *
*/

#include "encode.h"
#include <stdlib.h>       //to use malloc()

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
NODE unaryConvert(NODE oldNode){
      if(oldNode->type == TYFLOAT){
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
          if(oldNode->exprTypeTag == VARIABLE){
               id = st_lookup_id(oldNode->u.var_name);
            }
          else if(oldNode->exprTypeTag == DEREF){
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

/* this routine is used to generate assembly code for expression tree node*/
void geneAsmForNode(NODE node){
     if(node->exprTypeTag == CONST){
          if(node->type == TYSIGNEDLONGINT || node->type == TYUNSIGNEDINT){
                b_push_const_int(node->u.const_node.const_int_val);
             }
          else if(node->type == TYDOUBLE){
                b_push_const_double(node->u.const_node.const_double_val);
           }
       }
     else if(node->exprTypeTag == STRNG){
          b_push_const_string(node->u.string.str_const);
       }
     else if(node->exprTypeTag == VARIABLE){
          b_push_ext_addr(node->u.var_name);          //this will push the address of the varialbe on stack
      }
     else if(node->exprTypeTag == ASSIGN){
         geneAsmForNode(node->u.assign.left);
         geneAsmForNode(node->u.assign.right);
         b_assign(node->type);
         b_pop();
      }
     else if(node->exprTypeTag == BIN){
         geneAsmForNode(node->u.binop.left);
         geneAsmForNode(node->u.binop.right);
         b_arith_rel_op(node->u.binop.op_tag, node->type); 
      }
     else if(node->exprTypeTag == FUNC_ASSIGN){
         geneAsmForNode(node->u.assign.right);
         b_set_return(node->type);
      }
     else if(node->exprTypeTag == CONV){
         geneAsmForNode(node->u.convert.child);
         b_convert(node->u.convert.oldType, node->u.convert.newType);
      }
     else if(node->exprTypeTag == DEREF){
         geneAsmForNode(node->u.deref.child);
         b_deref(node->type);
      }
     else if(node->exprTypeTag == NEGATE){
         geneAsmForNode(node->u.negate.child);
         b_negate(node->type);
      }
     else if(node->exprTypeTag == FUNC){
          int argSize = 0;
          NODE argNode = node->u.func.arglist;
          if(argNode){
              while(argNode){
                 if(argNode->type == TYDOUBLE)
                     argSize += 8;
                 else
                     argSize += 4;
                 
                 argNode = argNode->next;
               }
            }
           b_alloc_arglist(argSize);         //allocate memory for argument list
           argNode = node->u.func.arglist;
           while(argNode){
               geneAsmForNode(argNode);
               b_load_arg(argNode->type);
               argNode = argNode->next;
            }
           b_funcall_by_name(node->u.func.funcName, node->type); 
      } 
  }




