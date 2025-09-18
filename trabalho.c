#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//conjunto de palavras-chave
const char *keywords[] ={

    "program","var","begin","end","if","then","else","procedure","integer",NULL

};

//função para verificar se uma string è uma palavra-chave
int iskeyword(const char *word){
    for (int i = 0; keywords[i] != NULL; i++){
        if (strcmp(word, keywords[i]) == 0)return 1;
    }
    return 0;
}

//função principal do lexer
void lexer(const char *code){
    int i = 0;
    while (code[i] != '\0'){
        char c = code[i];

     //ignorar espaços e quebras de linha
    if(isspace(c)){
        i++;
        continue;
    }
    //identificadores ou palavras-chave
    if(isalpha(c)){
        char buffer[100];
        int j = 0;
        while (isalnum(code[i])){
            buffer[j++] = code[i++];
        }
        buffer[j] = '\0';
        printf("NUM(%s)\n", buffer);
        continue;
    }
    //operadores 
    if( c == ':' && code[i+1]== '='){
        printf("ASSIGN(:=)\n");
        i +=2;
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
    i++;
    }
}

    //teste do autômato finito determinìsticos(AFD)
    int main(){
        const char *code =
        "program test; \n"
        "var x, y: integer; \n"
        "begin \n"
        "x := 10; \n"
        "y := x + 3; \n"
        "end.";
        
        lexer(code);
        return 0;
    }