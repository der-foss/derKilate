#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdarg.h>

#include "kilate/string.h"
#include "kilate/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  TOKEN_KEYWORD,     // work, return
  TOKEN_IDENTIFIER,  // hello, main
  TOKEN_STRING,      // "Hello world!"
  TOKEN_LPAREN,      // (
  TOKEN_RPAREN,      // )
  TOKEN_LBRACE,      // {
  TOKEN_RBRACE,      // }
  TOKEN_RARROW,      // ->
  TOKEN_LARROW,      // <-
  TOKEN_COLON,       // :
  TOKEN_TYPE,        // bool
  TOKEN_BOOL,        // true, false
  TOKEN_INT,         // 123
  TOKEN_FLOAT,       // 1.23
  TOKEN_LONG,        // 123l
  TOKEN_COMMA,       // ,,
  TOKEN_ASSIGN,      // =
  TOKEN_VAR,         // var
  TOKEN_LET,         // let
  TOKEN_EOF          // end of file.
} token_type;

typedef struct {
  token_type type;
  str text;

  size_t column;
  size_t line;
} token;

typedef vector token_vector;

token* token_make(token_type, str, size_t, size_t);

str tokentype_tostr(token_type);

typedef struct {
  str __input__;
  token_vector* tokens;

  size_t __pos__;
  size_t __column__;
  size_t __line__;
} lexer;

lexer* lexer_make(str);

void lexer_delete(lexer*);

void lexer_advance(lexer*);

str lexer_read_string(lexer*, bool*);

void lexer_tokenize(lexer*);

void lexer_error(lexer*, str, ...);

#ifdef __cplusplus
}
#endif

#endif