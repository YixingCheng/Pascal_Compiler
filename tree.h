/* tree.h */

#include "types.h"

typedef struct n {
   ST_ID id;
   struct n *next;
} ID_NODE, *ID_LIST;

INDEX_LIST addend(TYPE type, INDEX_LIST head);

ID_LIST addendIDList(ST_ID id, ID_LIST head);
