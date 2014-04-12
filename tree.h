/*
 *  tree.h for Pascal Compiler
 *  University of South Carolina
 *  author: Yixing Cheng, Zibo Meng, Ruofan Xia
 *  date: 4/7/2014
 *
 */

#include "types.h"
#include "backend-x86.h"

typedef enum{CONST, VARIABLE, BIN, CONV, DEREF, ASSIGN, NEGATE, FUNC, STRNG, FUNC_ASSIGN} TYPE_EXPR;

/* tree node for expression tree construction-- ethan  */
typedef struct exprtree_node{
    TYPE_EXPR exprTypeTag;      //type of the expression
    TYPETAG   type;             // in type.h, specify the type of a typename
    struct exprtree_node *next;
    union{
        union{
            int    const_int_val;
            double const_double_val;
        }const_node;           //const node
        char *var_name;         // variable node
        struct{
            B_ARITH_REL_OP op_tag;
            struct exprtree_node *left, *right;
        }binop;
        struct{
            TYPETAG oldType;
            TYPETAG newType;
            struct exprtree_node *child;
        }convert;
        struct{
            struct exprtree_node *child;
        }deref;
        struct{
            struct exprtree_node *left, *right;
        }assign;
        struct{
            struct exprtree_node *child;
        }negate;
        struct{
            char* funcName;
            struct exprtree_node *arglist;
        }func;
        struct{
            char* str_const;
        }string;
    }u;
 }TREENODE, *NODE;

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

/* this routine call routine in backend to generate assembly code for statement */
void geneAsmForStmt(NODE vari_or_func_access, NODE restofstmt);

/* this routine generate node is there's no assignment operator*/
NODE geneNodeIfNotAssign(char* tokenName);

/* generate assembly code for dispose() routine  */
void geneAsmForDispose(NODE parameter);

/* this routine generate node for a binary operator  */
NODE geneNodeForBiop(NODE left, B_ARITH_REL_OP biop, NODE right);
