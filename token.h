#ifndef TOKEN_H
#define TOKEN_H

/* classes de token (ajuste depois se o AFD do grupo usar outros nomes) */
typedef enum {
  TK_IDENT,
  TK_KEYWORD,
  TK_NUMBER_INT,
  TK_NUMBER_REAL,
  TK_OPERATOR,
  TK_SYMBOL,
  TK_STRING,
  TK_EOF,
  TK_UNKNOWN
} TokenType;

/* estrutura mínima exigida: nome/tipo, lexema, linha, coluna */
typedef struct {
  TokenType type;
  char      lexema[64];
  int       linha;
  int       coluna;
} Token;

#endif
