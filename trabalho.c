#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "symtable.h"

//função principal do lexer
void lexer(const char *code, TS* ts){
    int i = 0;
    int linha = 1, coluna = 1;
    while (code[i] != '\0'){
        char c = code[i];

     //ignorar espaços e quebras de linha
    if(c == '\n'){ i++; linha++; coluna = 1; continue; }
    if(isspace(c)){
        i++;
        coluna++;
        continue;
    }

    //identificadores ou palavras-chave
    if(isalpha(c)){
        char buffer[100];
        int j = 0;
        int col_start = coluna;
        while (isalnum(code[i])){
            buffer[j++] = code[i++];
            coluna++;
        }
        buffer[j] = '\0';
        if(ts_exists(ts, buffer)){
            printf("KEYWORD(%s)\n",buffer);
        }else{
            if(!ts_insert_ident(ts, buffer, linha, col_start)){
                printf("IDENT(%s)\n",buffer); /* já existia na TS (duplicado/keyword), mantém saída simples */
            }else{
                printf("IDENT(%s)\n",buffer);
            }
        }
        continue;
    }

    //nùmeros inteiros
    if(isdigit(c)){
        char buffer[100];
        int j = 0;
        while(isdigit(code[i])){
            buffer[j++] = code[i++];
            coluna++;
        }
        buffer[j]= '\0';
        printf("NUMBER(%s)\n",buffer);
        continue;

    }
    //comentários do tipo {...}
        if (c == '{') {
            i++; coluna++;
            while (code[i] != '}' && code[i] != '\0') {
                if (code[i] == '\n'){ linha++; coluna = 1; i++; }
                else { i++; coluna++; }
            }
            if (code[i] == '}') { i++; coluna++; }
            continue;
        }

        //comentários do tipo (*...*)
        if (c == '(' && code[i + 1] == '*') {
            i += 2; coluna += 2;
            while (!(code[i] == '*' && code[i + 1] == ')') && code[i] != '\0') {
                if (code[i] == '\n'){ linha++; coluna = 1; i++; }
                else { i++; coluna++; }
            }
            if (code[i] == '*' && code[i + 1] == ')') { i += 2; coluna += 2; }
            continue;
        }

    //operadores compostos
    if( c == ':' && code[i+1]== '='){
        printf("ASSIGN(:=)\n");
        i +=2; coluna += 2;
        continue;
    }
    if (c == '<') {
            if (code[i + 1] == '=') {
                printf("LEQ(<=)\n");
                i += 2; coluna += 2;
            } else if (code[i + 1] == '>') {
                printf("NEQ(<>)\n");
                i += 2; coluna += 2;
            } else {
                printf("LT(<)\n");
                i++; coluna++;
            }
            continue;
        }
        if (c == '>') {
            if (code[i + 1] == '=') {
                printf("GEQ(>=)\n");
                i += 2; coluna += 2;
            } else {
                printf("GT(>)\n");
                i++; coluna++;
            }
            continue;
        }
   
    //sìmbolo de 1 caractere
    switch(c){
        
        case '+' : printf("PLUS(+)\n"); break;
        case '-' : printf("MINUS(-)\n"); break;
        case '*' : printf("MULT(*)\n"); break;
        case '/' : printf("DIV(/)\n"); break;
        case '=' : printf("EQ(=)\n"); break;
        case '<' : printf("LT(<)\n"); break;
        case '>' : printf("GT(>)\n"); break;
        case ';' : printf("SEMICOLON(;)\n"); break;
        case ',' : printf("COMMA(,)\n"); break;
        case '(' : printf("LPAREN( ()\n"); break;
        case ')' : printf("RPAREN() ) \n"); break;
        case '.' : printf("DOT(.)\n"); break;
        case ':' : printf("COLON(:)\n"); break;
        default: printf("ERROR(%c)\n",c); break;

    }
    i++; coluna++;
    }
}

    //teste do autômato finito determinìsticos(AFD)
    int main(){
        TS* ts = ts_create();
        ts_preload_keywords(ts);

        const char *code =
        "program test; \n"
        "var x, y: integer; \n"
        "begin \n"
        "x := 10; \n"
        "y := x + 3; \n"
        "end.";
        
        lexer(code, ts);
        ts_print(ts);
        ts_free(ts);
        return 0;
    }
