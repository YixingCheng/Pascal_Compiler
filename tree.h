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
