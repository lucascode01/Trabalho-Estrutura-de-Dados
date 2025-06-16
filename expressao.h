#ifndef EXPRESSAO_H
#define EXPRESSAO_H

typedef struct {
    char posFixa[512];     // Expressão na forma pos-fixa
    char inFixa[512];      // Expressão na forma infixa
    float Valor;           // Valor numérico da expressão
} Expressao;

char *getFormaInFixa(char *Str);           // Retorna a forma infixa de Str (posfixa)
char *getFormaPosFixa(char *Str);          // Retorna a forma posfixa de Str (infixa)
float getValorPosFixa(char *StrPosFixa);   // Calcula valor de posfixa
float getValorInFixa(char *StrInFixa);     // Calcula valor de infixa

#endif
