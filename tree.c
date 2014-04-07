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
#include <assert.h>

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
