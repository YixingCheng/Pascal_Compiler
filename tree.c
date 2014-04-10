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



