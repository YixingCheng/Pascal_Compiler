/*
 *  tree.h for Pascal Compiler
 *  University of South Carolina
 *  author: Yixing Cheng, Zibo Meng, Ruofan Xia
 *  date: 4/7/2014
 *
 */

#include "types.h"

typedef struct n {
   ST_ID id;
   struct n *next;
} ID_NODE, *ID_LIST;


INDEX_LIST addend(TYPE type, INDEX_LIST head);

/* append new ID to ID list */
ID_LIST appendIDList(ST_ID id, ID_LIST head);

/* resolve unsolved pointer types*/
void resolvePointer();

/* chech whether an id is an existing type */
TYPE checkTypeName(ST_ID id);

/* build a subrang TYPE*/
TYPE createSubrange(int low, int high);

/* check typetag*/
BOOLEAN checkTypetag(TYPE funType);

/* create list of parameters function and procedure*/
PARAM_LIST createParalist(PARAM_LIST head);

/* append new formal parameter to parameter list */
PARAM_LIST appendParaList(PARAM_LIST head, PARAM_LIST newEntry);

/* create a formal parameter*/
PARAM_LIST createFormPara(ID_LIST idList, ST_ID id, BOOLEAN isRef);

/* create an Array type*/
TYPE createArray(INDEX_LIST indexList, TYPE type);

/* create array's index list*/
INDEX_LIST createIndexList(TYPE type, INDEX_LIST indexList);

/* this routine is used to declare variables*/
void declaVariable(ID_LIST idList, TYPE type);
