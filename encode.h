/*
 * encode.c of Pascal Compiler
 * University of South Carolina
 * author: Yixing Cheng, Zibo Meng, Ruofan Xia
 * date:4/09/2014
 *
*/

#include "types.h"
#include "tree.h"

void calSizeAlign(TYPE ty, int *align, unsigned int *size);

/* this routine is used to convert an old Node to a new type of node  */
NODE unaryConvert(NODE, oldNode);
