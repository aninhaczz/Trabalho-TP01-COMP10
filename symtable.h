#ifndef SYMTABLE_H
#define SYMTABLE_H
#include "token.h"

/* tipo opaco (detalhes escondidos no .c) */
typedef struct TS TS;

/* ciclo de vida */
TS*  ts_create(void);
void ts_free(TS* ts);

/* operações */
int  ts_exists(TS* ts, const char* lexema);                        // consulta
int  ts_insert_keyword(TS* ts, const char* kw);                    // uso interno
int  ts_insert_ident(TS* ts, const char* id, int linha, int coluna); // inserção

/* obrigatória (pré-carregar palavras reservadas) */
void ts_preload_keywords(TS* ts);

/* apoio/debug (útil para a Pessoa 4 imprimir ao final) */
void ts_print(TS* ts);

#endif
