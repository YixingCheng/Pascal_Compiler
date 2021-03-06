/****************************************************************************************
 *   									   		*
 *	CSCE 513 project1 & project 2 done by Yixing Cheng, Ruofan Xia, Zibo Meng	*
 *											*
 ****************************************************************************************/

/*A Bison parser for the programming language Pascal.
  Copyright (C) 1989-2002 Free Software Foundation, Inc.

  Authors: Jukka Virtanen <jtv@hut.fi>
           Helsinki University of Technology
           Computing Centre
           Finland

           Peter Gerwinski <peter@gerwinski.de>
           Essen, Germany

           Bill Cox <bill@cygnus.com> (error recovery rules)

           Frank Heckenbach <frank@pascal.gnu.de>

  This file is part of GNU Pascal.

  GNU Pascal is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published
  by the Free Software Foundation; either version 1, or (at your
  option) any later version.

  GNU Pascal is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with GNU Pascal; see the file COPYING. If not, write to the
  Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
  02111-1307, USA. */

/* Bison parser for ISO 7185 Pascal originally written on
 * 3 Feb 1987 by Jukka Virtanen <jtv@hut.fi>
 *
 * Modified for use at the University of South Carolina's CSCE 531
 * (Compiler Construction) course (Spring 2005) by Stephen Fenner
 * <fenner@cse.sc.edu>
 *
 * SHIFT/REDUCE CONFLICTS
 *
 * The dangling else will not cause a shift/reduce conflict - it's
 * solved by precedence rules.
 */

/*
 * gram.y for Pascal Compiler
 * University of South Carolina
 * author: Yixing Cheng, Zibo Meng, Ruofan Xia
 * date: 4/7/2014
 *
 *
 */

%{

#include "symtab.h"
#include "tree.h"
#include <string.h>
//#include "encode.h"

/* Cause the `yydebug' variable to be defined.  */
#define YYDEBUG 1

/* global variables */
int isDiv = 0;
int isMod = 0;
/* by Zibo */
int tmpDiv = 0;
int tmpMod = 0;

void set_yydebug(int);
void yyerror(const char *);

/* Like YYERROR but do call yyerror */
#define YYERROR1 { yyerror ("syntax error"); YYERROR; }

%}

/* Start symbol for the grammar */

%start pascal_program

/* The union representing a semantic stack entry */
%union {
    char            *y_string;
    long            y_int;
    double          y_real;
    TYPE            y_type;
    ST_ID           y_stid;
    INDEX_LIST      y_indexlist;
    ID_LIST         y_idlist;
    PARAM_LIST      y_paramlist;
    NODE            y_node;
    B_ARITH_REL_OP  y_op;
    PAS_FUNC 	    y_pasfunc;
    STORAGE_CLASS   y_sc;
    UNARYSIGN       y_sval;
}

%token LEX_ID

/* Reserved words. */

/* Reserved words in Standard Pascal */
%token LEX_ARRAY LEX_BEGIN LEX_CASE LEX_CONST LEX_DO LEX_DOWNTO LEX_END
%token LEX_FILE LEX_FOR LEX_FUNCTION LEX_GOTO LEX_IF LEX_LABEL LEX_NIL
%token LEX_OF LEX_PACKED LEX_PROCEDURE LEX_PROGRAM LEX_RECORD LEX_REPEAT
%token LEX_SET LEX_THEN LEX_TO LEX_TYPE LEX_UNTIL LEX_VAR LEX_WHILE LEX_WITH
%token LEX_FORWARD

/* The following ones are not tokens used in the parser.
 * However they are used in the same context as some tokens,
 * so assign unique numbers to them.
 */
%token pp_SIN pp_COS pp_EXP pp_LN pp_SQRT pp_ARCTAN rr_POW rr_EXPON
%token r_WRITE r_READ r_INITFDR r_LAZYTRYGET r_LAZYGET r_LAZYUNGET r_POW r_EXPON
%token z_ABS z_ARCTAN z_COS z_EXP z_LN z_SIN z_SQRT z_POW z_EXPON
%token set_card set_isempty set_equal set_le set_less set_in set_clear
%token set_include set_exclude set_include_range set_copy
%token set_intersection set_union set_diff set_symdiff
%token p_DONEFDR gpc_IOCHECK gpc_RUNTIME_ERROR

/* Redefinable identifiers. */

/* Redefinable identifiers in Standard Pascal */
%token p_INPUT p_OUTPUT p_REWRITE p_RESET p_PUT p_GET p_WRITE p_READ
%token p_WRITELN p_READLN p_PAGE p_NEW p_DISPOSE
%token p_ABS p_SQR p_SIN p_COS p_EXP p_LN p_SQRT p_ARCTAN
%token p_TRUNC p_ROUND p_PACK p_UNPACK p_ORD p_CHR p_SUCC p_PRED
%token p_ODD p_EOF p_EOLN p_MAXINT p_TRUE p_FALSE

/* Additional redefinable identifiers for Borland Pascal */
%token bp_RANDOM bp_RANDOMIZE BREAK CONTINUE

/* redefinable keyword extensions */
%token RETURN_ RESULT EXIT FAIL p_CLOSE CONJUGATE p_DEFINESIZE SIZEOF
%token BITSIZEOF ALIGNOF TYPEOF gpc_RETURNADDRESS gpc_FRAMEADDRESS
%token LEX_LABEL_ADDR

/* GPC internal tokens */
%token LEX_INTCONST LEX_STRCONST LEX_REALCONST
%token LEX_RANGE LEX_ELLIPSIS

/* We don't declare precedences for operators etc. We don't need
   them since our rules define precedence implicitly, and too many
   precedences increase the chances of real conflicts going unnoticed. */
%token LEX_ASSIGN
%token '<' '=' '>' LEX_IN LEX_NE LEX_GE LEX_LE
%token '-' '+' LEX_OR LEX_OR_ELSE LEX_CEIL_PLUS LEX_CEIL_MINUS LEX_FLOOR_PLUS LEX_FLOOR_MINUS
%token '/' '*' LEX_DIV LEX_MOD LEX_AND LEX_AND_THEN LEX_SHL LEX_SHR LEX_XOR LEX_CEIL_MULT LEX_CEIL_DIV LEX_FLOOR_MULT LEX_FLOOR_DIV
%token LEX_POW LEX_POWER LEX_IS LEX_AS
%token LEX_NOT

/* Various extra tokens */
%token LEX_EXTERNAL ucsd_STR p_MARK p_RELEASE p_UPDATE p_GETTIMESTAMP p_UNBIND
%token p_EXTEND bp_APPEND p_BIND p_SEEKREAD p_SEEKWRITE p_SEEKUPDATE LEX_SYMDIFF
%token p_ARG p_CARD p_EMPTY p_POSITION p_LASTPOSITION p_LENGTH p_TRIM p_BINDING
%token p_DATE p_TIME LEX_RENAME LEX_IMPORT LEX_USES LEX_QUALIFIED LEX_ONLY



%type <y_stid> typename new_identifier
%type <y_stid> function_heading
%type <y_int> LEX_INTCONST number constant
%type <y_string> new_identifier_1 LEX_ID
%type <y_string> identifier
%type <y_string> string combined_string LEX_STRCONST
%type <y_indexlist> array_index_list
%type <y_idlist> id_list
%type <y_type> type_denoter type_denoter_1
%type <y_type> new_pointer_type pointer_domain_type
%type <y_type> subrange_type array_type
%type <y_type> unpacked_structured_type new_structured_type
%type <y_type> ordinal_index_type new_ordinal_type
%type <y_type> functiontype new_procedural_type
%type <y_paramlist> optional_procedural_type_formal_parameter_list 
%type <y_paramlist> procedural_type_formal_parameter
%type <y_paramlist> procedural_type_formal_parameter_list
%type <y_paramlist> any_declaration_part
%type <y_paramlist> optional_par_formal_parameter_list
%type <y_node> rest_of_statement variable_or_function_access_maybe_assignment 
%type <y_node> expression simple_expression
%type <y_node> actual_parameter actual_parameter_list
%type <y_node> predefined_literal
%type <y_node> unsigned_number
%type <y_string> variable_access_or_typename
%type <y_op> relational_operator adding_operator multiplying_operator
%type <y_node> primary factor variable_or_function_access
%type <y_node> constant_literal variable_or_function_access_no_as
%type <y_node> variable_or_function_access_no_standard_function
%type <y_node> variable_or_function_access_no_id standard_functions
%type <y_pasfunc> rts_fun_onepar  rts_fun_parlist
%type <y_node> term signed_primary
%type <y_sc> directive_list directive
%type <y_sval> sign
%type <y_real> LEX_REALCONST

/* Precedence rules */

/* The following precedence declarations are just to avoid the dangling
   else shift-reduce conflict. We use prec_if rather than LEX_IF to
   avoid possible conflicts elsewhere involving LEX_IF going unnoticed. */
%nonassoc prec_if
%nonassoc LEX_ELSE

/* These tokens help avoid S/R conflicts from error recovery rules. */
%nonassoc lower_than_error
%nonassoc error

%%

/* Pascal parser starts here */

pascal_program:
    /* empty */
  {}| program_component_list
  {};

program_component_list:
    program_component
  {}| program_component_list program_component
  {};

program_component:
    main_program_declaration '.'
  { };

main_program_declaration:
    program_heading semi import_or_any_declaration_part{b_func_prologue("main");} 
statement_part
  {b_func_epilogue("main");};

program_heading:
    LEX_PROGRAM new_identifier optional_par_id_list
  {};

optional_par_id_list:
    /* empty */
  {}| '(' id_list ')'
  {};

id_list:
    new_identifier
  {
     $$ =  appendIDList($1,NULL);      //ethan's commit@4/7/2014
  }| id_list ',' new_identifier
  {
     $$ = appendIDList($3,$1);
  };


typename:
    LEX_ID
  {  $$ = st_enter_id($1);
  };

identifier:
    LEX_ID
  { $$ = $1;};


new_identifier:                 //this is the case id1,id2.. TYPENAME             
    new_identifier_1
  { $$ = st_enter_id($1);
  };

new_identifier_1:
    LEX_ID
/* Standard Pascal constants */
  {  $$ = $1;
  }| p_MAXINT
  {}| p_FALSE
  {}| p_TRUE
/* Standard Pascal I/O */
  {}| p_INPUT
  {}| p_OUTPUT
  {}| p_REWRITE
  {}| p_RESET
  {}| p_PUT
  {}| p_GET
  {}| p_WRITE
  {}| p_READ
  {}| p_WRITELN
  {}| p_READLN
  {}| p_PAGE
  {}| p_EOF
  {}| p_EOLN
/* Standard Pascal heap handling */
  {}| p_NEW
  {}| p_DISPOSE
/* Standard Pascal arithmetic */
  {}| p_ABS
  {}| p_SQR
  {}| p_SIN
  {}| p_COS
  {}| p_EXP
  {}| p_LN
  {}| p_SQRT
  {}| p_ARCTAN
  {}| p_TRUNC
  {}| p_ROUND
/* Standard Pascal transfer functions */
  {}| p_PACK
  {}| p_UNPACK
/* Standard Pascal ordinal functions */
  {}| p_ORD
  {}| p_CHR
  {}| p_SUCC
  {}| p_PRED
  {}| p_ODD
/* Other extensions */
  {}| BREAK
  {}| CONTINUE
  {}| RETURN_
  {}| RESULT
  {}| EXIT
  {}| FAIL
  {}| SIZEOF
  {}| BITSIZEOF
  {};

import_or_any_declaration_part:
    any_declaration_import_part
  {};

any_declaration_import_part:
    /* empty */
  {}| any_declaration_import_part any_or_import_decl
  {};

any_or_import_decl:
    import_part
  {}| any_decl
  {};

any_declaration_part:
    /* empty */
  { $$ = NULL; }| any_declaration_part any_decl
  {};

any_decl:
    simple_decl
  {}| function_declaration
  {};

simple_decl:
    label_declaration_part
  {}| constant_definition_part
  {}| type_definition_part
  {}| variable_declaration_part
  {};

/* Label declaration part */

label_declaration_part:
    LEX_LABEL label_list semi
  {};

label_list:
    label
  {}| label_list ',' label
  {};

/* Labels are returned as identifier nodes for compatibility with gcc */
label:
    LEX_INTCONST
  {}| new_identifier
  {};

/* constant definition part */

constant_definition_part:
    LEX_CONST constant_definition_list
  {};

constant_definition_list:
    constant_definition
  {}| constant_definition_list constant_definition
  {};

constant_definition:
    new_identifier '=' static_expression semi
  {};

constant:
    identifier
  {}| sign identifier
  {}| number
  {   $$ = $1;
  }| constant_literal
  {};

number:
    sign unsigned_number
  {  $$ = getIntFromConstNode($2); 
  }| unsigned_number
  {  $$ = getIntFromConstNode($1);
  };

unsigned_number:
    LEX_INTCONST
  {  $$ = geneNodeForIntConst($1);
  }| LEX_REALCONST
  { $$ = geneNodeForRealConst($1); };

sign:
    '+'
  { $$ = POSITIVE; }| '-'
  { $$ = NEGATIVE; };

constant_literal:
    combined_string
  { $$ = geneNodeForConLiter($1); }| predefined_literal
  { $$ = $1; };

predefined_literal:
    LEX_NIL
  {}| p_FALSE
  { $$ = geneNodeForBool(FALSE); }| p_TRUE
  { $$ = geneNodeForBool(TRUE); };

combined_string:
    string
  { $$ = $1; };

string:
    LEX_STRCONST
  { $$ = $1; } | string LEX_STRCONST
  { $$ = strcat($1, $2);};

type_definition_part:                //ethan's commit@4/7/2014 9:24pm
    LEX_TYPE type_definition_list semi
  {  resolvePointer();
  };

type_definition_list:
    type_definition
  {}| type_definition_list semi type_definition
  {};

type_definition:
    new_identifier '=' type_denoter{
     ST_DR dr = stdr_alloc();
     dr->tag = TYPENAME;
     dr->u.typename.type = $3;
     st_install($1,dr);
  };

type_denoter:
    typename{                       //ethan's commit@4/7/2014 9:58pm
      $$ = checkTypeName($1);
  }| type_denoter_1
  {
    $$ = $1;
  };

type_denoter_1:
    new_ordinal_type
  {}| new_pointer_type
  {   $$ = $1;
  }| new_procedural_type
  {   $$ = $1;
  }| new_structured_type
  {   $$ = $1; 
  };

new_ordinal_type:
    enumerated_type
  {}| subrange_type
  {  $$ = $1;
  };

enumerated_type:
    '(' enum_list ')'
  {};

enum_list:
    enumerator
  {}| enum_list ',' enumerator
  {};

enumerator:
    new_identifier
  {};

subrange_type:
    constant LEX_RANGE constant       //ethan's commit@4/7/2014 10:40pm
  {  $$ = createSubrange($1, $3);  
  };

new_pointer_type:
    pointer_char pointer_domain_type
  { $$ = $2;
  };

pointer_char:
    '^'
  {}| '@'
  {};

pointer_domain_type:
    new_identifier
  { $$ = ty_build_ptr($1,NULL);
  }| new_procedural_type
  { $$ = ty_build_ptr(NULL,$1);
  };

new_procedural_type:
    LEX_PROCEDURE optional_procedural_type_formal_parameter_list
  {  TYPE typroc = ty_build_func(ty_build_basic(TYVOID), $2, TRUE);
     $$ = typroc;
  }| LEX_FUNCTION optional_procedural_type_formal_parameter_list functiontype
  {  if(checkTypetag($3)){
       }
     else{
        error("Function return type must be simple type");
       }      
     $$ = ty_build_func($3, $2, TRUE);
  };

optional_procedural_type_formal_parameter_list:
    /* empty */
  { $$ = NULL;
  }| '(' procedural_type_formal_parameter_list ')'
  {  $$ = createParalist($2); };

procedural_type_formal_parameter_list:
    procedural_type_formal_parameter
  {  $$ = $1;
  }| procedural_type_formal_parameter_list semi procedural_type_formal_parameter
  {  $$ = appendParaList($1, $3);   
  };

procedural_type_formal_parameter:
    id_list
  {}| id_list ':' typename
  {  $$ = createFormPara($1, $3, FALSE);   
  }| LEX_VAR id_list ':' typename
  {  $$ = createFormPara($2, $4, TRUE);
  }
   | LEX_VAR id_list
  {};

new_structured_type:
    LEX_PACKED unpacked_structured_type
  {}| unpacked_structured_type
  { $$ = $1;
  };

unpacked_structured_type:
    array_type
  { $$ = $1;
  }| file_type
  {}| set_type
  {}| record_type
  {};

/* Array */

array_type:
    LEX_ARRAY '[' array_index_list ']' LEX_OF type_denoter
  {  $$ = createArray($3, $6);
  };

array_index_list:
    ordinal_index_type
  { $$ = createIndexList($1, NULL);   
  }| array_index_list ',' ordinal_index_type
  { $$ = createIndexList($3, $1);
  };


ordinal_index_type:
    new_ordinal_type
  {  $$ = $1;
  }| typename
  {};

/* FILE */
file_type:
    LEX_FILE direct_access_index_type LEX_OF type_denoter
  {};

direct_access_index_type:
    /* empty */
  {}| '[' ordinal_index_type ']'
  {};


/* sets */
set_type:
    LEX_SET LEX_OF type_denoter
  {};

record_type:
    LEX_RECORD record_field_list LEX_END
  {};

record_field_list:
    /* empty */
  {}| fixed_part optional_semicolon
  {}| fixed_part semi variant_part
  {}| variant_part
  {};

fixed_part:
    record_section
  {}| fixed_part semi record_section
  {};

record_section:
    id_list ':' type_denoter
  {};

variant_part:
    LEX_CASE variant_selector LEX_OF variant_list rest_of_variant
  {};

rest_of_variant:
    optional_semicolon
  {}| case_default '(' record_field_list ')' optional_semicolon
  {};

variant_selector:
    new_identifier ':' variant_type
  {}| variant_type
  {};

variant_type:
    typename
  {}| new_ordinal_type
  {};

variant_list:
    variant
  {}| variant_list semi variant
  {};

variant:
    case_constant_list ':' '(' record_field_list ')'
  {};

case_constant_list:
    one_case_constant
  {}| case_constant_list ',' one_case_constant
  {};

one_case_constant:
    static_expression
  {}| static_expression LEX_RANGE static_expression
  {};

/* variable declaration part */

variable_declaration_part:
    LEX_VAR variable_declaration_list
  {};

variable_declaration_list:
    variable_declaration
  {}| variable_declaration_list variable_declaration
  {};

variable_declaration:
    id_list ':' type_denoter semi
  {  declaVariable($1, $3);  }
function_declaration:
    function_heading semi directive_list semi
  {  funcDeclandDireList($1, $3);  }| function_heading semi {
      ST_ID id = $1;
      int temp;
      ST_DR entry = st_lookup(id, &temp);
      if(entry){
            entry->tag = FDECL;
         }
      st_enter_block();
      b_init_formal_param_offset();
  } any_declaration_part {
      b_func_prologue(st_get_id_str($1));
      ST_ID id = $1;
      int temp;
      ST_DR entry = st_lookup(id, &temp);
      PARAM_LIST plist;
      BOOLEAN chargs;
      TYPE ty = ty_query_func(entry->u.decl.type, &plist, &chargs);
      TYPETAG tytag = ty_query(ty);
      if(tytag != TYVOID){
            b_alloc_return_value();
         }
      b_alloc_local_vars(0);
  } statement_part semi
  {   ST_ID id = $1;
      int temp;
      ST_DR entry = st_lookup(id, &temp);
      PARAM_LIST plist;
      BOOLEAN chargs;
      TYPE ty = ty_query_func(entry->u.decl.type, &plist, &chargs);
      TYPETAG tytag = ty_query(ty);
      if(tytag !=TYVOID){
           b_prepare_return(tytag);
        }
      b_func_epilogue(st_get_id_str($1));
      st_exit_block();
   };

function_heading:
    LEX_PROCEDURE new_identifier optional_par_formal_parameter_list
  { $$ = funcHeadingForProc($2, $3); }| LEX_FUNCTION new_identifier optional_par_formal_parameter_list functiontype
  { $$ = funcHeadingForFunc($2, $3, $4);};

directive_list:
    directive
  { $$ = $1; }| directive_list semi directive
  {};

directive:
    LEX_FORWARD
  { STORAGE_CLASS sc = NO_SC;
    $$ = sc;}| LEX_EXTERNAL
  { STORAGE_CLASS sc = NO_SC;
    $$ = sc;};

functiontype:
    /* empty */
  {}| ':' typename
  {  $$ = checkTypeName($2);  };

/* parameter specification section */

optional_par_formal_parameter_list:
    /* empty */
  { PARAM_LIST paralist = NULL;
    $$ = paralist;
  }| '(' formal_parameter_list ')'
  {};

formal_parameter_list:
    formal_parameter
  {}| formal_parameter_list semi formal_parameter
  {};

formal_parameter:
    id_list ':' parameter_form
  {}| LEX_VAR id_list ':' parameter_form
  {}| function_heading
  {}| id_list ':' conformant_array_schema
  {}| LEX_VAR id_list ':' conformant_array_schema
  {};

parameter_form:
    typename
  {}| open_array
  {};

conformant_array_schema:
    packed_conformant_array_schema
  {}| unpacked_conformant_array_schema
  {};

typename_or_conformant_array_schema:
    typename
  {}| packed_conformant_array_schema
  {}| unpacked_conformant_array_schema
  {};

packed_conformant_array_schema:
    LEX_PACKED LEX_ARRAY '[' index_type_specification ']' LEX_OF typename_or_conformant_array_schema
  {};

unpacked_conformant_array_schema:
    LEX_ARRAY '[' index_type_specification_list ']' LEX_OF typename_or_conformant_array_schema
  {};

index_type_specification:
    new_identifier LEX_RANGE new_identifier ':' typename
  {};

index_type_specification_list:
    index_type_specification
  {}| index_type_specification_list semi index_type_specification
  {};

open_array:
    LEX_ARRAY LEX_OF typename
  {};

statement_part:
    compound_statement
  {};

compound_statement:
    LEX_BEGIN statement_sequence LEX_END
  {};

statement_sequence:
    statement
  {}| statement_sequence semi statement
  {};

statement:
    label ':' unlabelled_statement
  {}| unlabelled_statement
  {};

unlabelled_statement:
    structured_statement
  {}| simple_statement
  {};

structured_statement:
    compound_statement
  {}| with_statement
  {}| conditional_statement
  {}| repetitive_statement
  {};

with_statement:
    LEX_WITH structured_variable_list LEX_DO statement
  {};

structured_variable_list:
    structured_variable
  {}| structured_variable_list ',' structured_variable
  {};

structured_variable:
    variable_or_function_access
  {};

conditional_statement:
    if_statement
  {}| case_statement
  {};

simple_if:
    LEX_IF boolean_expression LEX_THEN statement
  {};

if_statement:
    simple_if LEX_ELSE statement
  {}| simple_if %prec prec_if
  {};

case_statement:
    LEX_CASE expression LEX_OF case_element_list optional_semicolon_or_else_branch LEX_END
  {};

optional_semicolon_or_else_branch:
    optional_semicolon
  {}| case_default statement_sequence
  {};

case_element_list:
    case_element
  {}| case_element_list semi case_element
  {};

case_element:
    case_constant_list ':' statement
  {};

case_default:
    LEX_ELSE
  {}| semi LEX_ELSE
  {};

repetitive_statement:
    repeat_statement
  {}| while_statement
  {}| for_statement
  {};

repeat_statement:
    LEX_REPEAT statement_sequence LEX_UNTIL boolean_expression
  {};

while_statement:
    LEX_WHILE boolean_expression LEX_DO statement
  {};

for_statement:
    LEX_FOR variable_or_function_access LEX_ASSIGN expression for_direction expression LEX_DO statement
  {};

for_direction:
    LEX_TO
  {}| LEX_DOWNTO
  {};

simple_statement:
    empty_statement
  {}| goto_statement
  {}| assignment_or_call_statement
  {}| standard_procedure_statement
  {}| statement_extensions
  {};

empty_statement:
    /* empty */ %prec lower_than_error
  {};

goto_statement:
    LEX_GOTO label
  {};

/* function calls */

optional_par_actual_parameter_list:
    /* empty */
  {}| '(' actual_parameter_list ')'
  {};

actual_parameter_list:
    actual_parameter
  { $$ = geneNodeForActuParaList($1);}| actual_parameter_list ',' actual_parameter
  { $$ = appendActuPara($1, $3);};

actual_parameter:
    expression
  {  $$ = $1;};

/* ASSIGNMENT and procedure calls */

assignment_or_call_statement:
    variable_or_function_access_maybe_assignment rest_of_statement
  { geneAsmForStmt($1, $2);};       //ethan commit @4/10/2014 2:36pm

variable_or_function_access_maybe_assignment:
    identifier
  { $$ = geneNodeIfNotAssign($1);   //ethan commit @4/11/2014 1:01am
}| address_operator variable_or_function_access
  {}| variable_or_function_access_no_id
  {};

rest_of_statement:
    /* Empty */
  { $$ = NULL;}| LEX_ASSIGN expression
  {$$ = $2;};

standard_procedure_statement:
    rts_proc_onepar '(' actual_parameter ')'
  {}| rts_proc_parlist '(' actual_parameter_list ')'
  {}| p_WRITE optional_par_write_parameter_list
  {}| p_WRITELN optional_par_write_parameter_list
  {}| p_READ optional_par_actual_parameter_list
  {}| p_READLN optional_par_actual_parameter_list
  {}| p_PAGE optional_par_actual_parameter_list
  {}| ucsd_STR '(' write_actual_parameter_list ')'
  {}| p_DISPOSE '(' actual_parameter ')'
  { geneAsmForDispose($3);     //ethan commit @4/11/2014 1:35am
  }| p_DISPOSE '(' actual_parameter ',' actual_parameter_list ')'
  {};

optional_par_write_parameter_list:
    /* empty */
  {}| '(' write_actual_parameter_list ')'
  {};

write_actual_parameter_list:
    write_actual_parameter
  {}| write_actual_parameter_list ',' write_actual_parameter
  {};

write_actual_parameter:
    actual_parameter
  {}| actual_parameter ':' expression
  {}| actual_parameter ':' expression ':' expression
  {};

/* run time system calls with one parameter */
rts_proc_onepar:
    p_PUT
  {}| p_GET
  {}| p_MARK
  {}| p_RELEASE
  {}| p_CLOSE
  {}| p_UPDATE
  {}| p_GETTIMESTAMP
  {}| p_UNBIND
  {};

rts_proc_parlist:
    p_REWRITE     /* Up to three args */
  {}| p_RESET       /* Up to three args */
  {}| p_EXTEND      /* Up to three args */
  {}| bp_APPEND     /* Up to three args */
  {}| p_PACK        /* Three args */
  {}| p_UNPACK      /* Three args */
  {}| p_BIND        /* Two args */
  {}| p_SEEKREAD
  {}| p_SEEKWRITE
  {}| p_SEEKUPDATE
  {}| p_DEFINESIZE  /* Two args */
  {}| LEX_AND           /* Two args */
  {}| LEX_OR            /* Two args */
  {}| LEX_NOT           /* One arg */
  {}| LEX_XOR        /* Two args */
  {}| LEX_SHL           /* Two args */
  {}| LEX_SHR           /* Two args */
  {};

statement_extensions:
    return_statement
  {}| continue_statement
  {}| break_statement
  {};

return_statement:
    RETURN_
  {}| RETURN_ expression
  {}| EXIT
  {}| FAIL
  {};

break_statement:
    BREAK
  {};

continue_statement:
    CONTINUE
  {};

variable_access_or_typename:
    variable_or_function_access_no_id
  {}| LEX_ID
  { $$ = $1;};

index_expression_list:
    index_expression_item
  {}| index_expression_list ',' index_expression_item
  {};

index_expression_item:
    expression
  {}| expression LEX_RANGE expression
  {};

/* expressions */

static_expression:
    expression
  {};

boolean_expression:
    expression
  {};

expression:
    expression relational_operator simple_expression
  { $$ = geneNodeForBiop($1, $2, $3); }| expression LEX_IN simple_expression
  {}| simple_expression
  { $$ = $1;};

simple_expression:
    term
  { $$ = $1;}| simple_expression adding_operator term
  { $$ = geneNodeForAdd($1, $2, $3); }| simple_expression LEX_SYMDIFF term
  {}| simple_expression LEX_OR term
  {}| simple_expression LEX_XOR term
  {};

term:
    signed_primary
  { $$ = $1;}| term multiplying_operator signed_primary
  { tmpDiv = isDiv;
    tmpMod = isMod;
    if(isDiv == 1) isDiv = 0;
    else if(isMod == 1) isMod = 0;
    $$ = geneNodeForMulti($1, $2, $3, isDiv, isMod); //modified by Zibo
   }| term LEX_AND signed_primary
  {};

signed_primary:
    primary
  { $$ = $1;}| sign signed_primary
  { $$ = geneSignedPrimNode($1, $2);};

/* edited by Zibo */
primary:
    factor
  { $$ = $1;
   }| primary LEX_POW factor
  {}| primary LEX_POWER factor
  {}| primary LEX_IS typename
  {};

signed_factor:
    factor
  {}| sign signed_factor
  {};

/* edited by Zibo */
factor:
    variable_or_function_access
  { $$ = $1;
   }| constant_literal
  { $$ = $1;
   }| unsigned_number
  { $$ = $1;
   }| set_constructor
  {}| LEX_NOT signed_factor
  {}| address_operator factor
  {};

address_operator:
    '@'
  {};

/* edited by Zibo */
variable_or_function_access:
    variable_or_function_access_no_as
  { $$ = $1;
   }| variable_or_function_access LEX_AS typename
  {};

/* edited by Zibo */
variable_or_function_access_no_as:
    variable_or_function_access_no_standard_function
  { $$ = $1;
   }| standard_functions
  { $$ = $1;
   };

/* edited by Zibo */
variable_or_function_access_no_standard_function:
    identifier
  { $$ = geneNodeForNoStdFunc($1);    //edited by Zibo
   }| variable_or_function_access_no_id
  { $$ = $1;
   };

/* edited by Zibo */
variable_or_function_access_no_id:
    p_OUTPUT
  {}| p_INPUT
  {}| variable_or_function_access_no_as '.' new_identifier
  {}| '(' expression ')'
  { $$ = $2;
   }| variable_or_function_access pointer_char
  { $$ = geneNodeForFuncPointer($1);  //edited by Zibo
   }| variable_or_function_access '[' index_expression_list ']'
  {}| variable_or_function_access_no_standard_function '(' actual_parameter_list ')'
  { NODE node = $1;
    node->u.func.arglist = $3;
    $$ = node;             //edited by Zibo
   }| p_NEW '(' variable_access_or_typename ')'
  { $$ = geneNodeForVarPointer($3);  //edited by Zibo
   };

set_constructor:
    '[' ']'
  {}| '[' set_constructor_element_list ']'
  {};

set_constructor_element_list:
    member_designator
  {}| set_constructor_element_list ',' member_designator
  {};

member_designator:
    expression
  {}| expression LEX_RANGE expression
  {};

standard_functions:
    rts_fun_onepar '(' actual_parameter ')'
  { $$ = geneNodeForOneParam($1,$3);  //edited by Zibo
   }| rts_fun_optpar optional_par_actual_parameter
    rts_fun_onepar '('actual_parameter ')'
  {}| rts_fun_optpar optional_par_actual_parameter
  {}| rts_fun_parlist '(' actual_parameter_list ')'
  { $$ = geneNodeForParamList($1,$3); //edited by Zibo
   };

optional_par_actual_parameter:
    /* empty */
  {}|  '(' actual_parameter ')'
  {};

rts_fun_optpar:
    p_EOF
  {}| p_EOLN
  {};


/* edited by Zibo */
rts_fun_onepar:
    p_ABS
  {}| p_SQR
  {}| p_SIN
  {}| p_COS
  {}| p_EXP
  {}| p_LN
  {}| p_SQRT
  {}| p_ARCTAN
  {}| p_ARG
  {}| p_TRUNC
  {}| p_ROUND
  {}| p_CARD
  {}| p_ORD
  { PAS_FUNC pf = ORD;
    $$ = pf;
   }| p_CHR
  { PAS_FUNC pf = CHR;
    $$ = pf;
   }| p_ODD
  {}| p_EMPTY
  {}| p_POSITION
  {}| p_LASTPOSITION
  {}| p_LENGTH
  {}| p_TRIM
  {}| p_BINDING
  {}| p_DATE
  {}| p_TIME
  {};

rts_fun_parlist:
    p_SUCC        /* One or two args */
  { PAS_FUNC pf = SUCC;
    $$ = pf;
   }| p_PRED        /* One or two args */
  { PAS_FUNC pf = PRED;
    $$ = pf;
   };

relational_operator:
    LEX_NE
  { B_ARITH_REL_OP operator= B_NE;
     $$ = operator;
   }| LEX_LE
  { B_ARITH_REL_OP operator= B_LE;
     $$ = operator;
   }| LEX_GE
  { B_ARITH_REL_OP operator= B_GE;
     $$ = operator;
   }| '='
  { B_ARITH_REL_OP operator= B_EQ;
     $$ = operator;
   }| '<'
  { B_ARITH_REL_OP operator= B_LT;
     $$ = operator;
   }| '>'
  { B_ARITH_REL_OP operator= B_GT;
     $$ = operator;
   };

multiplying_operator:
    LEX_DIV
  { B_ARITH_REL_OP operator= B_DIV;
    isDiv = 1;
    $$ = operator;
   }| LEX_MOD
  { B_ARITH_REL_OP operator= B_MOD;
    isMod = 1;
    $$ = operator;
   }| '/'
  { B_ARITH_REL_OP operator= B_DIV;
    $$ = operator;
   }| '*'
  { B_ARITH_REL_OP operator= B_MULT;
    $$ = operator;
   };

adding_operator:
    '-'
  { B_ARITH_REL_OP operator= B_SUB;
     $$ = operator;
   }| '+'
  { B_ARITH_REL_OP operator= B_ADD;
     $$ = operator;
   };

semi:
    ';'
  {};

optional_semicolon:
    /* empty */
  {}| ';'
  {};

optional_rename:
    /* empty */
  {}| LEX_RENAME new_identifier
  {};

import_part:
    LEX_IMPORT import_specification_list semi
  {}| LEX_USES uses_list semi
  {};

import_specification_list:
    import_specification
  {}| import_specification_list semi import_specification
  {};

uses_list:
    import_specification
  {}| uses_list ',' import_specification
  {};

import_specification:
    new_identifier optional_access_qualifier optional_import_qualifier optional_unit_filename
  {};

optional_access_qualifier:
    /* Empty */
  {}| LEX_QUALIFIED
  {};

optional_import_qualifier:
    /* Empty */
  {}| '(' import_clause_list ')'
  {}| LEX_ONLY '(' import_clause_list ')'
  {};

optional_unit_filename:
    /* Empty */
  {}| LEX_IN combined_string
  {};

import_clause_list:
    import_clause
  {}| import_clause_list ',' import_clause
  {};

import_clause:
    new_identifier optional_rename
  {};

%%

void yyerror(const char *msg)
{
    error(msg);
}

/* Sets the value of the 'yydebug' variable to VALUE.
   This is a function so we don't have to have YYDEBUG defined
   in order to build the compiler.  */
void
set_yydebug (value)
     int value;
{
#if YYDEBUG != 0
  yydebug = value;
#else
  warning ("YYDEBUG not defined.");
#endif
}
