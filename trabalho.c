#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "symtable.h"

/* Lexer que lê de FILE* e escreve tokens em <input>.lex */

static int is_keyword(const char* s){
  const char* kw[] = {"program","var","integer","real","begin","end","if","then","else","while","do", NULL};
  for(int i=0; kw[i]; ++i){
    if (strcasecmp(kw[i], s)==0) return 1;
  }
  return 0;
}

static void write_token_file(FILE* f, const char* name, const char* lex, int linha, int col){
  fprintf(f, "<%s,%s,%d,%d>\n", name, lex, linha, col);
}

static void maybe_add_ident(TS* ts, const char* lex, int linha, int coluna){
  if (!ts_exists(ts, lex)) ts_insert_ident(ts, lex, linha, coluna);
}

void lexer(FILE* in, const char* inputname, TS* ts){
  int linha=1, col=0;
  char outname[256];
  snprintf(outname, sizeof(outname), "%s.lex", inputname);
  FILE* out = fopen(outname, "w");
  if (!out){ perror("fopen"); return; }

  int c;
  while((c=fgetc(in))!=EOF){
    if (c==' '||c=='\t'||c=='\r'){ col++; continue; }
    if (c=='\n'){ linha++; col=0; continue; }

    // identificadores / palavras reservadas
    if (isalpha(c)){
      int startcol = col+1;
      char buf[128]; int i=0;
      buf[i++]=c; col++;
    while((c=fgetc(in))!=EOF && isalnum(c)){
      if (i< (int)sizeof(buf)-1) { buf[i++]=c; }
      col++;
    }
      if (c!=EOF) ungetc(c,in);
      buf[i]=0;
      if (is_keyword(buf)){
        // palavra reservada
        write_token_file(out, buf, buf, linha, startcol); // exemplo: <PROGRAM,program,...>
        if (!ts_exists(ts,buf)) ts_insert_keyword(ts,buf);
      } else {
        // identificador
        write_token_file(out,"ID",buf,linha,startcol);
        maybe_add_ident(ts,buf,linha,startcol);
      }
      continue;
    }

    // números (inteiros ou reais)
    if (isdigit(c)){
      int startcol=col+1;
      char buf[128]; int i=0; int is_real=0;
      buf[i++]=c; col++;
      while((c=fgetc(in))!=EOF){
        if (isdigit(c)){ if(i<127)buf[i++]=c; col++; continue;}
        if (c=='.'){ is_real=1; if(i<127)buf[i++]=c; col++;
          while((c=fgetc(in))!=EOF && isdigit(c)){ if(i<127)buf[i++]=c; col++; }
          if(c!=EOF) ungetc(c,in);
          break;
        }
        if (c=='E'||c=='e'){ is_real=1; if(i<127)buf[i++]=c; col++;
          c=fgetc(in); if(c=='+'||c=='-'){ if(i<127)buf[i++]=c; col++; } else if(c!=EOF) ungetc(c,in);
          while((c=fgetc(in))!=EOF && isdigit(c)){ if(i<127)buf[i++]=c; col++; }
          if(c!=EOF) ungetc(c,in);
          break;
        }
        if(c!=EOF) ungetc(c,in);
        break;
      }
      buf[i]=0;
      write_token_file(out, is_real?"NUM_REAL":"NUM_INT", buf, linha,startcol);
      continue;
    }

    // strings -> erro léxico (não suportadas)
    if (c=='\''||c=='"'){
      int startcol=col+1;
      fprintf(stderr,"Erro lexico: string nao suportada em %d:%d\n",linha,startcol);
      fprintf(out,"## ERROR: string literal not supported at %d:%d\n",linha,startcol);
      continue;
    }

    // comentários -> erro léxico (não suportados)
    if(c=='{'){
      int startcol=col+1;
      fprintf(stderr,"Erro lexico: comentario nao suportado em %d:%d\n",linha,startcol);
      fprintf(out,"## ERROR: comment not supported at %d:%d\n",linha,startcol);
      continue;
    }
    if(c=='('){
      int nc=fgetc(in);
      if(nc=='*'){
        int startcol=col+1;
        fprintf(stderr,"Erro lexico: comentario nao suportado em %d:%d\n",linha,startcol);
        fprintf(out,"## ERROR: comment not supported at %d:%d\n",linha,startcol);
        continue;
      } else {
        if(nc!=EOF) ungetc(nc,in);
      }
    }

    // operadores e símbolos
    int startcol=col+1; col++;
    if(c==':'){ int nc=fgetc(in); 
      if(nc=='='){ write_token_file(out,"OP_ASS",":=",linha,startcol); col++; } 
      else { if(nc!=EOF) ungetc(nc,in); write_token_file(out,"DOIS_PONTOS",":",linha,startcol);} 
      continue; }
    if(c=='<'){ int nc=fgetc(in); 
      if(nc=='>'){ write_token_file(out,"OP_NE","<>",linha,startcol); col++; } 
      else if(nc=='='){ write_token_file(out,"OP_LE","<=",linha,startcol); col++; } 
      else { if(nc!=EOF) ungetc(nc,in); write_token_file(out,"OP_LT","<",linha,startcol);} 
      continue; }
    if(c=='>'){ int nc=fgetc(in); 
      if(nc=='='){ write_token_file(out,"OP_GE",">=",linha,startcol); col++; } 
      else { if(nc!=EOF) ungetc(nc,in); write_token_file(out,"OP_GT",">",linha,startcol);} 
      continue; }
    if(c=='='){ write_token_file(out,"OP_EQ","=",linha,startcol); continue; }
    if(c=='+'){ write_token_file(out,"OP_AD","+",linha,startcol); continue; }
    if(c=='-'){ write_token_file(out,"OP_MIN","-",linha,startcol); continue; }
    if(c=='*'){ write_token_file(out,"OP_MUL","*",linha,startcol); continue; }
    if(c=='/'){ write_token_file(out,"OP_DIV","/",linha,startcol); continue; }
    if(c==';'){ write_token_file(out,"SMB_SEM",";",linha,startcol); continue; }
    if(c==','){ write_token_file(out,"SMB_COM",",",linha,startcol); continue; }
    if(c=='('){ write_token_file(out,"SMB_OPA","(",linha,startcol); continue; }
    if(c==')'){ write_token_file(out,"SMB_CPA",")",linha,startcol); continue; }
    if(c=='.'){ write_token_file(out,"SMB_DOT",".",linha,startcol); continue; }

    // caractere inválido
    { char tmp[4]={c,0,0,0}; 
      fprintf(stderr,"Erro lexico: caractere invalido '%c' em %d:%d\n",c,linha,startcol);
      fprintf(out,"## ERROR: invalid character '%c' at %d:%d\n",c,linha,startcol);
      write_token_file(out,"UNKNOWN",tmp,linha,startcol); }
  }
  fclose(out);
}

int main(int argc, char** argv){
  if(argc<2){ fprintf(stderr,"Uso: %s arquivo.mp\n",argv[0]); return 1; }
  FILE* in=fopen(argv[1],"r");
  if(!in){ perror("fopen"); return 1; }
  TS* ts=ts_create();
  ts_preload_keywords(ts);
  lexer(in, argv[1], ts);
  fclose(in);
  ts_print(ts);
  ts_free(ts);
  return 0;
}
