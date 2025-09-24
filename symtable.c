#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symtable.h"

/* ===== Hash com encadeamento e comparação case-insensitive ===== */

#define NBUC 211  /* número de buckets (primo ajuda) */

typedef struct Node {
  Token        tok;
  struct Node* next;
} Node;

struct TS { Node* b[NBUC]; };

/* --- util: hash/tolower/igualdade case-insensitive --- */
static unsigned h_ci(const char* s){
  unsigned long h=5381;
  for(; *s; ++s){
    int c=tolower((unsigned char)*s);
    h=((h<<5)+h)+c; /* h*33 + c */
  }
  return (unsigned)(h%NBUC);
}

static void tolower_copy(char* dst, size_t n, const char* src){
  size_t i=0;
  for(; src[i] && i+1<n; ++i) dst[i]=(char)tolower((unsigned char)src[i]);
  dst[i]='\0';
}

static int eq_ci(const char* a, const char* b){
  for(; *a && *b; ++a, ++b){
    int ca=tolower((unsigned char)*a), cb=tolower((unsigned char)*b);
    if(ca!=cb) return 0;
  }
  return *a==*b;
}

static Token make_token(TokenType t, const char* lex, int ln, int col){
  Token x;
  x.type=t;
  tolower_copy(x.lexema, sizeof(x.lexema), lex); /* normaliza para lowercase */
  x.linha=ln; x.coluna=col;
  return x;
}

/* ===== API ===== */

TS* ts_create(void){
  TS* ts=(TS*)calloc(1, sizeof(TS));
  return ts;
}

void ts_free(TS* ts){
  if(!ts) return;
  for(int i=0;i<NBUC;i++){
    Node* n=ts->b[i];
    while(n){ Node* nx=n->next; free(n); n=nx; }
  }
  free(ts);
}

static int ts_insert_raw(TS* ts, Token t){
  unsigned h=h_ci(t.lexema);
  for(Node* n=ts->b[h]; n; n=n->next){
    if(eq_ci(n->tok.lexema, t.lexema)) return 0; /* já existe */
  }
  Node* nn=(Node*)malloc(sizeof(Node));
  nn->tok=t; nn->next=ts->b[h]; ts->b[h]=nn;
  return 1;
}

int ts_exists(TS* ts, const char* lexema){
  unsigned h=h_ci(lexema);
  for(Node* n=ts->b[h]; n; n=n->next){
    if(eq_ci(n->tok.lexema, lexema)) return 1;
  }
  return 0;
}

int ts_insert_keyword(TS* ts, const char* kw){
  Token t = make_token(TK_KEYWORD, kw, 0, 0);
  return ts_insert_raw(ts, t);
}

/* regra: NÃO inserir se já existir (duplicado) ou se for keyword */
int ts_insert_ident(TS* ts, const char* id, int linha, int coluna){
  if(ts_exists(ts, id)) return 0;                   /* já existe (ident/keyword) */
  Token t = make_token(TK_IDENT, id, linha, coluna);
  return ts_insert_raw(ts, t);
}

void ts_preload_keywords(TS* ts){
  const char* kw[]={
    "program","var","integer","real","begin","end","if","then","else","while","do"
  };
  for(size_t i=0;i<sizeof(kw)/sizeof(kw[0]);++i) ts_insert_keyword(ts, kw[i]);
}

void ts_print(TS* ts){
  puts("=== Tabela de Símbolos ===");
  for(int i=0;i<NBUC;i++){
    for(Node* n=ts->b[i]; n; n=n->next){
      const char* k =
        n->tok.type==TK_KEYWORD ? "KEYWORD" :
        n->tok.type==TK_IDENT   ? "IDENT"   :
        n->tok.type==TK_NUMBER_INT ? "NUM_INT" :
        "OUTRO";
      printf("<%s, %s, %d, %d>\n", k, n->tok.lexema, n->tok.linha, n->tok.coluna);
    }
  }
}
