/*
 *  tree.c for Pascal Compiler
 *  University of South Carolina
 *  author: Yixing Cheng, Zibo Meng, Ruofan Xia
 *  date: 4/7/2014
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "types.h"
#include "defs.h"
#include "symtab.h"
//#include "encode.h"
#include <assert.h>

extern void calSizeAlign(TYPE ty, int *align, unsigned int *size);
extern NODE unaryConvert(NODE oldNode);
extern void geneAsmForNode(NODE node);


PARAM_LIST addbeginParamList(ST_ID id, TYPE ty, BOOLEAN is_ref, PARAM_LIST *head);

INDEX_LIST addend(TYPE type, INDEX_LIST head)
{
   INDEX_LIST end, pointer;
   
   end = (INDEX_LIST)malloc(sizeof(INDEX));
   end->type = type;
   end->next = NULL;
   
   if(head == NULL)
   {
      head = end;
   }
   else   
   {
      pointer = head;
      while(pointer->next != NULL)
         pointer = pointer->next;

      pointer->next = end;
   }
   return head;
}


ID_LIST appendIDList(ST_ID id, ID_LIST head)
{
   ID_LIST end, listEntry;   
   listEntry = head;
   end = (ID_LIST)malloc(sizeof(struct n));
   assert(end);

   end->id = id;
   end->next = NULL;
   
   if(head == NULL)
   {
      head = end;
   }
   else   
   {
      while(listEntry->next != NULL){
          listEntry = listEntry->next;
        }

      listEntry->next = end;
   }
   return head;
}

/* resolve unresolved pointer types */
void resolvePointer(){
     TYPE unresolPtr = ty_get_unresolved();
     ST_ID id;
     TYPE next;
     while(unresolPtr != NULL){
           TYPE resolvedType = ty_query_ptr(unresolPtr, &id, &next);
           if(!resolvedType){
                int temp;
                ST_DR entry = st_lookup(id, &temp);
                if(entry){
                    ty_resolve_ptr(unresolPtr, entry->u.typename.type); 
                  }
                else{
                    error("Unresolved type name: \"%s\"", st_get_id_str(id));
                  }
             }
       }

  }

/* chech whether an id is an existing type */
TYPE checkTypeName(ST_ID id){
      int temp;
      ST_DR entry = st_lookup(id, &temp);
      if(entry)
          return entry->u.typename.type;
      else{
          error("Undeclared type name: \"%s\"", st_get_id_str(id));
          return ty_build_basic(TYERROR);
        }
   }

/* build a subrang TYPE*/
TYPE createSubrange(int low, int high){
      TYPE subrange;
      if(low > high){
         error("Empty subrange in array index");
         error("Illegal index type (ignored)");
         subrange = ty_build_basic(TYERROR);
         }
      else{
         subrange = ty_build_subrange(ty_build_basic(TYSIGNEDLONGINT), low, high);
         }
      return subrange;
   }

/* check typetag*/
BOOLEAN checkTypetag(TYPE funType){
       TYPETAG typetag = ty_query(funType);
       if(typetag != TYSIGNEDINT &&
          typetag != TYDOUBLE &&
          typetag != TYSIGNEDLONGINT &&
          typetag != TYUNSIGNEDCHAR &&
          typetag != TYSIGNEDCHAR &&
          typetag != TYPTR &&
          typetag != TYSUBRANGE){
             return FALSE;
            }
       else{
             return TRUE;
           }
   }

/* create list of parameters function and procedure*/
PARAM_LIST createParalist(PARAM_LIST head){
       PARAM_LIST nextEntry, entry;
       entry = head;
       while(entry){
             if(!checkTypetag(entry->type));
                  error("Parameter type must be a simple type");
             nextEntry = entry->next;
             while(nextEntry){
                   if(entry->id == nextEntry->id)
                      error("Duplicate parameter name: \"%s\"", st_get_id_str(entry->id));
                   nextEntry = nextEntry->next; 
               }
            entry = entry->next;
          }
       return head;
  }

/* append new formal parameter to parameter list */
PARAM_LIST appendParaList(PARAM_LIST head, PARAM_LIST newEntry){
        PARAM_LIST temp;
        temp = head;
        while(temp->next){
            temp = temp->next;
          }
        temp->next = newEntry;
        newEntry->next = NULL;
        return head;
   }

/* create a formal parameter*/
PARAM_LIST createFormPara(ID_LIST idList, ST_ID id, BOOLEAN isRef){
         int temp;
         PARAM_LIST paraList = NULL;
         ST_DR entry = st_lookup(id, &temp);
         TYPE typeParaList ;
         if((entry != NULL) & (entry->tag == TYPENAME)){
              typeParaList = entry->u.typename.type;
            }
         else{
              error("Undeclared type name: \"%s\"", st_get_id_str(id));
              typeParaList = ty_build_basic(TYERROR);
            }
         
         addbeginParamList(idList->id, typeParaList, isRef, &paraList);
         idList = idList->next;
         while(idList){
             addbeginParamList(idList->id, typeParaList, isRef, &paraList);
             idList = idList->next;
           }
         return paraList;
    }

/* create an Array type*/
TYPE createArray(INDEX_LIST indexList, TYPE type){
      if((ty_query(type) == TYERROR) || (ty_query(type) == TYFUNC)){
            error("Data type expected for array elements");
            return ty_build_basic(TYERROR);
          }
      else{
             TYPE arrayType = ty_build_array(type, indexList);
             return arrayType;
          }
    }

/* create array's index list*/
INDEX_LIST createIndexList(TYPE type, INDEX_LIST indexList){

       if(!indexList){
          if(ty_query(type) != TYERROR){
               INDEX_LIST newIndexList = addend(type, NULL);
               return newIndexList;
            }
          else{
               return NULL;
            }
          }
       else{
          if(ty_query(type) != TYERROR){
               INDEX_LIST newIndexList = addend(type, indexList);
               return newIndexList;
            }
          else{
               return NULL;
            }
          }   
       
   }

/* this routine is used to declare variables*/
void declaVariable(ID_LIST idList, TYPE type){
       if((ty_query(type) == TYERROR) || (ty_query(type)) == TYFUNC)
           error("Variable(s) must be of data type");
       ID_LIST localidList = idList;
       
       resolvePointer();
        
       while(localidList){
             ST_DR entry;
             entry = stdr_alloc();
             entry->tag = GDECL;
             entry->u.decl.type = type;
             entry->u.decl.sc = NO_SC;
             entry->u.decl.is_ref = FALSE;
             if(ty_query(type) == TYERROR)
                 entry->u.decl.err = TRUE;
             else
                 entry->u.decl.err = FALSE;
             
             int temp;
             ST_DR checkEntry = st_lookup(localidList->id, &temp);
             if(!checkEntry)
                error("Duplicate variable declaration: \"%s\"", st_get_id_str(localidList->id));
             else{
                st_install(localidList->id, entry);
                int size = 0;
                int align = 0;
                calSizeAlign(entry->u.decl.type, &align, &size);
                TYPETAG typetag = ty_query(entry->u.decl.type);
                char *str = st_get_id_str(localidList->id);
                if((ty_query(type) != TYERROR) && (ty_query(type) != TYFUNC)){
                       b_global_decl(str, align, size);
                       b_skip(size);
                   }
             }
            localidList = localidList->next;
         } 
   }

PARAM_LIST addbeginParamList(ST_ID id, TYPE ty, BOOLEAN is_ref, PARAM_LIST *head)
{
   PARAM_LIST start;
   
   start = (PARAM_LIST)malloc(sizeof(PARAM));
   start->id = id;
   start->type = ty;
   start->sc = NO_SC;
   start->err = FALSE;
   start->is_ref = is_ref;

   start->next = NULL;
   
   if(*head == NULL)
   {
      *head = start;
   }
   else   
   {
      start->next = *head;
      *head = start;
   }
   return *head;
}

/* this routine call routine in backend to generate assembly code for statement */
void geneAsmForStmt(NODE vari_or_func_access, NODE restofstmt){
       if(restofstmt){                             // is rest of the statment is not NULL, then this is an assignment statement
            NODE localNode = malloc(sizeof(struct exprtree_node));
            assert(localNode);           //then build an assignment node
            localNode->exprTypeTag = ASSIGN;
            localNode->u.assign.right = restofstmt;
            localNode->u.assign.left  = vari_or_func_access;
            localNode->type = vari_or_func_access->type;
            if(localNode->u.assign.right->exprTypeTag == VARIABLE ){       // if the right node is a variable, we need an explict deref node
                 NODE derefNode = malloc(sizeof(struct exprtree_node));
                 derefNode->exprTypeTag = DEREF;
                 derefNode->type = localNode->u.assign.right->type;
                 derefNode->u.deref.child = localNode->u.assign.right;
                 localNode->u.assign.right = derefNode;
                 localNode->u.assign.right = unaryConvert(localNode->u.assign.right);
              }
            if(localNode->u.assign.left->type != localNode->u.assign.right->type){
                 NODE tempNode = malloc(sizeof(struct exprtree_node));
                 tempNode->exprTypeTag = CONV;
                 tempNode->type = localNode->u.assign.left->type;
                 tempNode->u.convert.oldType = localNode->u.assign.right->type;
                 tempNode->u.convert.newType = localNode->u.assign.left->type;
                 tempNode->u.convert.child   = localNode->u.assign.right;
                 tempNode->u.assign.right    = tempNode;
              }
            if(localNode->u.assign.left->exprTypeTag == FUNC){   //maybeshould be right here
                 localNode->exprTypeTag = FUNC_ASSIGN;
              }
            geneAsmForNode(localNode);
        } 
       else{
            geneAsmForNode(vari_or_func_access);
        }      
   }

/* this routine generate node is there's no assignment operator*/
NODE geneNodeIfNotAssign(char* tokenName){
      ST_ID id = st_lookup_id(tokenName);
      int temp;
      ST_DR entry = st_lookup(id, &temp);
      NODE newNode = malloc(sizeof(struct exprtree_node));
      TYPETAG typetag = ty_query(entry->u.decl.type);
      if(typetag == TYFUNC){                    // in case of function access
           newNode->exprTypeTag = FUNC;
           PARAM_LIST paraList;
           BOOLEAN arguCheck;
           TYPE returnType = ty_query_func(entry->u.decl.type, &paraList, &arguCheck);
           TYPETAG returnTag = ty_query(returnType);
           newNode->type = returnTag;
           newNode->u.func.funcName = tokenName;
           newNode->next = NULL;
           newNode->u.func.arglist = NULL;
        }
      else{                                   // in case of variable
           newNode->exprTypeTag = VARIABLE;
           newNode->type = typetag;
           newNode->u.var_name = tokenName; 
        }
      
      if(newNode->type == TYPTR){
           NODE derefNode = malloc(sizeof(struct exprtree_node));
           derefNode->exprTypeTag = DEREF;
           derefNode->type = newNode->type;
           derefNode->u.deref.child = newNode;
           newNode = derefNode;  
        }
      return newNode;
   }

/* generate assembly code for dispose() routine  */
void geneAsmForDispose(NODE parameter){
      NODE disposeNode = malloc(sizeof(struct exprtree_node));
      disposeNode->exprTypeTag = FUNC;
      disposeNode->type = TYVOID;
      disposeNode->u.func.funcName = "free";
      disposeNode->next = NULL;
      parameter->next   = NULL;
      disposeNode->u.func.arglist = parameter;
      geneAsmForNode(disposeNode);
  }

/* this routine generate node for a binary operator */
NODE geneNodeForBiop(NODE left, B_ARITH_REL_OP biop, NODE right){
      NODE biopNode = malloc(sizeof(struct exprtree_node));
      biopNode->exprTypeTag = BIN;
      biopNode->u.binop.left = left;
      biopNode->u.binop.right = right;
      biopNode->u.binop.op_tag = biop;
      
      if(biopNode->u.binop.left->exprTypeTag == VARIABLE){       //if left node is a variable
           NODE derefNode = malloc(sizeof(struct exprtree_node));
           derefNode->exprTypeTag = DEREF;
           derefNode->type = biopNode->u.binop.left->type;
           derefNode->u.deref.child = biopNode->u.binop.left; 
           biopNode->u.binop.left = derefNode;
           biopNode->u.binop.left = unaryConvert(biopNode->u.binop.left);
        }
      if(biopNode->u.binop.right->exprTypeTag == VARIABLE){      //if right node is a variable
           NODE derefNode1 = malloc(sizeof(struct exprtree_node));
           derefNode1->exprTypeTag = DEREF;
           derefNode1->type = biopNode->u.binop.right->type;
           derefNode1->u.deref.child = biopNode->u.binop.right; 
           biopNode->u.binop.right = derefNode1;
           biopNode->u.binop.right = unaryConvert(biopNode->u.binop.right);
        }
      if(biopNode->u.binop.left->type == biopNode->u.binop.right->type){   //if type of operands are the same
           if(biopNode->u.binop.left->type == TYUNSIGNEDCHAR || biopNode->u.binop.left->type == TYSIGNEDCHAR)         //if operands are char
            {
                NODE convNode = malloc(sizeof(struct exprtree_node));
                convNode->exprTypeTag = CONV;
                convNode->u.convert.oldType = biopNode->u.binop.left->type;
                convNode->u.convert.newType = TYSIGNEDLONGINT;
                convNode->u.convert.child   = biopNode->u.binop.left;
                convNode->type = TYSIGNEDLONGINT;
                biopNode->u.binop.left      = convNode;

                NODE rightconvNode = malloc(sizeof(struct exprtree_node));
                rightconvNode->exprTypeTag = CONV;
                rightconvNode->u.convert.oldType = biopNode->u.binop.right->type;
                rightconvNode->u.convert.newType = TYSIGNEDLONGINT;
                rightconvNode->u.convert.child   = biopNode->u.binop.right;
                convNode->type = TYSIGNEDLONGINT;
                biopNode->u.binop.right      = rightconvNode; 
            }
           else{
                biopNode->type=biopNode->u.binop.left->type;
            }
        }
      else if(biopNode->u.binop.left->type == TYDOUBLE && biopNode->u.binop.right->type == TYSIGNEDLONGINT){
             if(biopNode->u.binop.right->exprTypeTag == CONST){
                   biopNode->u.binop.right->type = TYDOUBLE;
                   biopNode->u.binop.right->u.const_node.const_double_val = biopNode->u.binop.right->u.const_node.const_int_val;
                   biopNode->type = biopNode->u.binop.left->type;
              }
             else{      //need to explicitly convert from int to double
                   NODE itodNode = malloc(sizeof(struct exprtree_node));
                   itodNode->exprTypeTag = CONV;
                   itodNode->u.convert.oldType = biopNode->u.binop.right->type;
                   itodNode->u.convert.newType = TYDOUBLE;
                   itodNode->u.convert.child   = biopNode->u.binop.right;
                   itodNode->type = TYDOUBLE;
                   biopNode->u.binop.right      = itodNode;
                   biopNode->type = biopNode->u.binop.left->type;
              }
          }
      // if left operand is long and right operand is double 
      else if(biopNode->u.binop.left->type == TYSIGNEDLONGINT && biopNode->u.binop.right->type == TYDOUBLE ){
             if(biopNode->u.binop.left->exprTypeTag == CONST){
                   biopNode->u.binop.left->type = TYDOUBLE;
                   biopNode->u.binop.left->u.const_node.const_double_val = biopNode->u.binop.left->u.const_node.const_int_val;
                   biopNode->type = biopNode->u.binop.right->type;
              }
             else{
                   NODE leftitodNode = malloc(sizeof(struct exprtree_node));
                   leftitodNode->exprTypeTag = CONV;
                   leftitodNode->u.convert.oldType = biopNode->u.binop.left->type;
                   leftitodNode->u.convert.newType = TYDOUBLE;
                   leftitodNode->u.convert.child   = biopNode->u.binop.left;
                   leftitodNode->type              = TYDOUBLE;
                   biopNode->u.binop.left      = leftitodNode;
                   biopNode->type = biopNode->u.binop.right->type;
              }      
           }     
      
      NODE convBiopNode = malloc(sizeof(struct exprtree_node));
      convBiopNode->exprTypeTag = CONV;
      convBiopNode->u.convert.oldType = biopNode->type;
      convBiopNode->u.convert.newType = TYSIGNEDCHAR;
      convBiopNode->u.convert.child   = biopNode;
      convBiopNode->type              = TYSIGNEDCHAR;
      biopNode = convBiopNode;
      return biopNode;
  }

/* this routine generate node of adding operator  */
NODE geneNodeForAdd(NODE left, B_ARITH_REL_OP addop, NODE right){
      NODE addNode = malloc(sizeof(struct exprtree_node));
      addNode->exprTypeTag = BIN;
      addNode->u.binop.left = left;
      addNode->u.binop.right = right;
      addNode->u.binop.op_tag = addop;  
      
      if(addNode->u.binop.left->exprTypeTag == VARIABLE){       //if left node is a variable
           NODE addderefNode = malloc(sizeof(struct exprtree_node));
           addderefNode->exprTypeTag = DEREF;
           addderefNode->type = addNode->u.binop.left->type;
           addderefNode->u.deref.child = addNode->u.binop.left; 
           addNode->u.binop.left = addderefNode;
           addNode->u.binop.left = unaryConvert(addNode->u.binop.left);
        }
      if(addNode->u.binop.right->exprTypeTag == VARIABLE){      //if right node is a variable
           NODE addderefNode1 = malloc(sizeof(struct exprtree_node));
           addderefNode1->exprTypeTag = DEREF;
           addderefNode1->type = addNode->u.binop.right->type;
           addderefNode1->u.deref.child = addNode->u.binop.right; 
           addNode->u.binop.right = addderefNode1;
           addNode->u.binop.right = unaryConvert(addNode->u.binop.right);
        }
      if(addNode->u.binop.left->type == TYSIGNEDLONGINT && addNode->u.binop.right->type == TYSIGNEDLONGINT){
            addNode->type = TYSIGNEDLONGINT;
        }      
      else if(addNode->u.binop.left->type == TYDOUBLE && addNode->u.binop.right->type == TYSIGNEDLONGINT){
            if(addNode->u.binop.right->exprTypeTag == CONST){
                  addNode->u.binop.right->type = TYDOUBLE;
                  addNode->u.binop.right->u.const_node.const_double_val = addNode->u.binop.right->u.const_node.const_int_val;
                  addNode->type = addNode->u.binop.left->type;
              }
            else{
                   NODE additodNode = malloc(sizeof(struct exprtree_node));
                   additodNode->exprTypeTag = CONV;
                   additodNode->u.convert.oldType = addNode->u.binop.right->type;
                   additodNode->u.convert.newType = TYDOUBLE;
                   additodNode->u.convert.child   = addNode->u.binop.right;
                   additodNode->type = TYDOUBLE;
                   addNode->u.binop.right      = additodNode;
                   addNode->type = addNode->u.binop.left->type; 
             }
         }
      else if(addNode->u.binop.left->type == TYSIGNEDLONGINT && addNode->u.binop.right->type == TYDOUBLE){
            if(addNode->u.binop.left->exprTypeTag == CONST){
                  addNode->u.binop.left->type = TYDOUBLE;
                  addNode->u.binop.left->u.const_node.const_double_val = addNode->u.binop.left->u.const_node.const_int_val;
                  addNode->type = addNode->u.binop.left->type;
              }
            else{
                   NODE leftadditodNode = malloc(sizeof(struct exprtree_node));
                   leftadditodNode->exprTypeTag = CONV;
                   leftadditodNode->u.convert.oldType = addNode->u.binop.left->type;
                   leftadditodNode->u.convert.newType = TYDOUBLE;
                   leftadditodNode->u.convert.child   = addNode->u.binop.left;
                   leftadditodNode->type = TYDOUBLE;
                   addNode->u.binop.left      = leftadditodNode;
                   addNode->type = addNode->u.binop.left->type; 
             } 
        }
     else if(addNode->u.binop.left->type == TYDOUBLE && addNode->u.binop.right->type == TYDOUBLE)         {
             addNode->type = TYDOUBLE;  
         }

     return addNode; 
  }

/* this routine generate node for mutiplying operator */
NODE geneNodeForMulti(NODE left, B_ARITH_REL_OP multiop, NODE right){
       NODE multiNode = malloc(sizeof(struct exprtree_node));
       multiNode->exprTypeTag = BIN;
       multiNode->u.binop.left = left;
       multiNode->u.binop.right = right;
       multiNode->u.binop.op_tag = multiop;

       if(multiNode->u.binop.left->exprTypeTag == VARIABLE){       //if left node is a variable
           NODE multiderefNode = malloc(sizeof(struct exprtree_node));
           multiderefNode->exprTypeTag = DEREF;
           multiderefNode->type = multiNode->u.binop.left->type;
           multiderefNode->u.deref.child = multiNode->u.binop.left; 
           multiNode->u.binop.left = multiderefNode;
           multiNode->u.binop.left = unaryConvert(multiNode->u.binop.left);
        }
      if(multiNode->u.binop.right->exprTypeTag == VARIABLE){      //if right node is a variable
           NODE multiderefNode1 = malloc(sizeof(struct exprtree_node));
           multiderefNode1->exprTypeTag = DEREF;
           multiderefNode1->type = multiNode->u.binop.right->type;
           multiderefNode1->u.deref.child = multiNode->u.binop.right; 
           multiNode->u.binop.right = multiderefNode1;
           multiNode->u.binop.right = unaryConvert(multiNode->u.binop.right);
        }
      if(isDiv == 1){
           isDiv = 0;
           if(multiNode->u.binop.left->type == TYSIGNEDLONGINT && multiNode->u.binop.right->type == TYSIGNEDLONGINT){
                        multiNode->type = TYSIGNEDLONGINT;
                   }
           else{
                    multiNode->type = TYSIGNEDLONGINT;
              }
       }
      else if(isMod == 1){
           isMod = 0;
           if(multiNode->u.binop.left->type == TYSIGNEDLONGINT && multiNode->u.binop.right->type == TYSIGNEDLONGINT){
                 multiNode->type = TYSIGNEDLONGINT;
             }
           else{
                 multiNode->type = TYSIGNEDLONGINT;
             }
       }
      else{
           if(multiNode->u.binop.left->type == TYSIGNEDLONGINT && multiNode->u.binop.right->type == TYSIGNEDLONGINT){
                multiNode->type = TYSIGNEDLONGINT;
             }
           else if(multiNode->u.binop.left->type == TYDOUBLE && multiNode->u.binop.right->type ==TYSIGNEDLONGINT){
                if(multiNode->u.binop.right->exprTypeTag == CONST){
                     multiNode->u.binop.right->type = TYDOUBLE;
                     multiNode->u.binop.right->u.const_node.const_double_val = multiNode->u.binop.right->u.const_node.const_int_val;
                     multiNode->type = multiNode->u.binop.left->type;
                  }
                else{
                    NODE multiItoDNode = malloc(sizeof(struct exprtree_node));
                    multiItoDNode->exprTypeTag = CONV;
                    multiItoDNode->u.convert.oldType = multiNode->u.binop.right->type;
                    multiItoDNode->u.convert.newType = TYDOUBLE;
                    multiItoDNode->u.convert.child   = multiNode->u.binop.right;
                    multiItoDNode->type              = TYDOUBLE;
                    multiNode->u.binop.right         = multiItoDNode;
                    multiNode->type                  = multiNode->u.binop.left->type;
                  }
             }
           else if(multiNode->u.binop.left->type == TYSIGNEDLONGINT && multiNode->u.binop.right->type == TYDOUBLE){
                     if(multiNode->u.binop.left->exprTypeTag == CONST){
                          multiNode->u.binop.left->type = TYDOUBLE;
                          multiNode->u.binop.left->u.const_node.const_double_val = multiNode->u.binop.left->u.const_node.const_int_val;
                          multiNode->type = multiNode->u.binop.right->type;
                      }
                else{
                       NODE leftmultiItoDNode = malloc(sizeof(struct exprtree_node));
                       leftmultiItoDNode->exprTypeTag = CONV;
                       leftmultiItoDNode->u.convert.oldType = multiNode->u.binop.left->type;
                       leftmultiItoDNode->u.convert.newType = TYDOUBLE;
                       leftmultiItoDNode->u.convert.child   = multiNode->u.binop.left;
                       leftmultiItoDNode->type              = TYDOUBLE;
                       multiNode->u.binop.left         = leftmultiItoDNode;
                       multiNode->type                  = multiNode->u.binop.right->type;
                  }
              }
           else if(multiNode->u.binop.left->type == TYDOUBLE && multiNode->u.binop.right->type == TYDOUBLE){
               multiNode->type = TYDOUBLE;
          }
       }
    
      return multiNode;
   }
