#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdarg.h>

#include "kilate/lexer.h"
#include "kilate/native.h"
#include "kilate/node.h"
#include "kilate/string.h"
#include "kilate/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  token_vector* tokens;
  node_vector* nodes;
  size_t __pos__;
} parser;

parser* parser_make(token_vector*);

void parser_delete(parser*);

void parser_delete_params(str_vector*);

token* parser_consume(parser*, token_type);

node* parser_find_function(parser*, str);

str parser_tokentype_to_str(token_type);

str parser_nodevaluetype_to_str(node_valuetype);

node_valuetype parser_tokentype_to_nodevaluetype(parser*,
                                                         token*);

node_valuetype parser_str_to_nodevaluetype(str);

node* parser_parse_statement(parser*);

node_fnparam_vector* parser_parse_fnparams(parser* parser);

void parser_fn_validate_params(node*,
                                   node_fnparam_vector*,
                                   token*);

node* parser_parse_call_node(parser*, token*);

node* parser_parse_import(parser*);

void parser_parse_program(parser*);

node* parser_parse_function(parser*);

void parser_error(token*, str, ...);

#ifdef __cplusplus
}
#endif

#endif