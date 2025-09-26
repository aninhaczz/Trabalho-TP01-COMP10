#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symtable.h"

#define NBUC 211

typedef struct Node {
  Token tok;
  struct Node* next;
} Node;

struct TS { Node* b[NBUC]; };

static unsigned hash(const char* s){
  unsigned h = 5381;
  for(const unsigned char* p=(const unsigned char*)s; *p; ++p) h = ((h<<5)+h) + *p;
  return h % NBUC;
}

TS* ts_create(void){
  TS* ts = (TS*)malloc(sizeof(TS));
  if (!ts) return NULL;
  for(int i=0;i<NBUC;i++) ts->b[i]=NULL;
  return ts;
}

void ts_free(TS* ts){
  if(!ts) return;
  for(int i=0;i<NBUC;i++){
    Node* n = ts->b[i];
    while(n){
      Node* nx = n->next;
      free(n);
      n = nx;
    }
  }
  free(ts);
}

/* consulta se lexema existe na tabela (case-insensitive) */
int ts_exists(TS* ts, const char* lexema){
  if(!ts || !lexema) return 0;
  unsigned h = hash(lexema);
  for(Node* n = ts->b[h]; n; n=n->next){
    if (strcasecmp(n->tok.lexema, lexema) == 0) return 1;
  }
  return 0;
}

/* insere palavra-chave (uso interno) */
int ts_insert_keyword(TS* ts, const char* kw){
  if(!ts || !kw) return 0;
  if (ts_exists(ts, kw)) return 0;
  unsigned h = hash(kw);
  Node* n = (Node*)malloc(sizeof(Node));
  memset(n,0,sizeof(Node));
  n->next = ts->b[h];
  n->tok.type = TK_KEYWORD;
  strncpy(n->tok.lexema, kw, sizeof(n->tok.lexema)-1);
  n->tok.linha = 0; n->tok.coluna = 0;
  ts->b[h] = n;
  return 1;
}

/* insere identificador (apenas se não existir) */
int ts_insert_ident(TS* ts, const char* id, int linha, int coluna){
  if(!ts || !id) return 0;
  if (ts_exists(ts, id)) return 0;
  unsigned h = hash(id);
  Node* n = (Node*)malloc(sizeof(Node));
  memset(n,0,sizeof(Node));
  n->next = ts->b[h];
  n->tok.type = TK_IDENT;
  strncpy(n->tok.lexema, id, sizeof(n->tok.lexema)-1);
  n->tok.linha = linha; n->tok.coluna = coluna;
  ts->b[h] = n;
  return 1;
}

/* pré-carrega as palavras reservadas */
void ts_preload_keywords(TS* ts){
  const char* kw[] = {"program","var","integer","real","begin","end","if","then","else","while","do", NULL};
  for(int i=0; kw[i]; ++i) ts_insert_keyword(ts, kw[i]);
}

/* imprime conteúdo da tabela */
void ts_print(TS* ts){
  puts("=== Tabela de Símbolos ===");
  for(int i=0;i<NBUC;i++){
    for(Node* n=ts->b[i]; n; n=n->next){
      const char* k =
        n->tok.type==TK_KEYWORD ? "KEYWORD" :
        n->tok.type==TK_IDENT   ? "IDENT"   :
        n->tok.type==TK_NUMBER_INT ? "NUM_INT" :
        n->tok.type==TK_NUMBER_REAL ? "NUM_REAL" :
        "OUTRO";
      printf("<%s, %s, %d, %d>\n", k, n->tok.lexema, n->tok.linha, n->tok.coluna);
    }
  }
}
