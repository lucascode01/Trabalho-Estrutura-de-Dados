#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "expressao.h"

#define MAX 512

typedef struct {
    float dados[MAX];
    int topo;
} Pilha;

void init(Pilha *p) { p->topo = -1; }
void push(Pilha *p, float v) { p->dados[++p->topo] = v; }
float pop(Pilha *p) { return p->dados[p->topo--]; }

int prioridade(char *op) {
    if (strcmp(op, "^") == 0) return 4;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 3;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 2;
    return 0;
}

int ehOperador(char *s) {
    return strcmp(s, "+") == 0 || strcmp(s, "-") == 0 || strcmp(s, "*") == 0 ||
           strcmp(s, "/") == 0 || strcmp(s, "%") == 0 || strcmp(s, "^") == 0;
}

int ehFuncao(char *s) {
    return strcmp(s, "sen") == 0 || strcmp(s, "cos") == 0 || strcmp(s, "tg") == 0 ||
           strcmp(s, "log") == 0 || strcmp(s, "raiz") == 0;
}

float grausParaRad(float graus) {
    return graus * M_PI / 180.0;
}

char *getFormaPosFixa(char *infixa) {
    static char posfixa[MAX];
    char pilha[MAX][32];
    int topo = -1, j = 0;
    char token[32];
    int i = 0;

    while (infixa[i]) {
        if (isspace(infixa[i])) {
            i++;
            continue;
        } else if (isdigit(infixa[i]) || infixa[i] == '.') {
            int k = 0;
            while (isdigit(infixa[i]) || infixa[i] == '.')
                token[k++] = infixa[i++];
            token[k] = '\0';
            sprintf(&posfixa[j], "%s ", token);
            j += strlen(token) + 1;
        } else if (isalpha(infixa[i])) {
            int k = 0;
            while (isalpha(infixa[i]))
                token[k++] = infixa[i++];
            token[k] = '\0';
            strcpy(pilha[++topo], token);
        } else if (infixa[i] == '(') {
            strcpy(pilha[++topo], "(");
            i++;
        } else if (infixa[i] == ')') {
            while (topo >= 0 && strcmp(pilha[topo], "(") != 0) {
                sprintf(&posfixa[j], "%s ", pilha[topo--]);
                j += strlen(pilha[topo + 1]) + 1;
            }
            if (topo >= 0 && strcmp(pilha[topo], "(") == 0) topo--;
            if (topo >= 0 && ehFuncao(pilha[topo])) {
                sprintf(&posfixa[j], "%s ", pilha[topo--]);
                j += strlen(pilha[topo + 1]) + 1;
            }
            i++;
        } else {
            char op[2] = { infixa[i++], '\0' };
            while (topo >= 0 && prioridade(pilha[topo]) >= prioridade(op)) {
                sprintf(&posfixa[j], "%s ", pilha[topo--]);
                j += strlen(pilha[topo + 1]) + 1;
            }
            strcpy(pilha[++topo], op);
        }
    }
    while (topo >= 0) {
        sprintf(&posfixa[j], "%s ", pilha[topo--]);
        j += strlen(pilha[topo + 1]) + 1;
    }
    posfixa[j] = '\0';
    return posfixa;
}

char *getFormaInFixa(char *posfixa) {
    static char infixa[MAX];
    char copia[MAX];
    strcpy(copia, posfixa);

    char *token = strtok(copia, " ");
    char pilha[100][128];
    int topo = -1;

    while (token) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            strcpy(pilha[++topo], token);
        } else if (ehFuncao(token)) {
            if (topo >= 0) {
                char resultado[128];
                sprintf(resultado, "%s(%s)", token, pilha[topo--]);
                strcpy(pilha[++topo], resultado);
            }
        } else if (ehOperador(token)) {
            if (topo >= 1) {
                char b[128], a[128], resultado[128];
                strcpy(b, pilha[topo--]);
                strcpy(a, pilha[topo--]);
                sprintf(resultado, "(%s %s %s)", a, token, b);
                strcpy(pilha[++topo], resultado);
            }
        }
        token = strtok(NULL, " ");
    }

    if (topo == 0)
        strcpy(infixa, pilha[topo]);
    else
        strcpy(infixa, "(erro na conversão)");

    return infixa;
}

float getValorPosFixa(char *StrPosFixa) {
    Pilha p;
    init(&p);
    char expr[MAX];
    strcpy(expr, StrPosFixa);

    char *token = strtok(expr, " ");
    while (token) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            push(&p, atof(token));
        } else if (ehFuncao(token)) {
            float a = pop(&p);
            if (strcmp(token, "sen") == 0)
                push(&p, sin(grausParaRad(a)));
            else if (strcmp(token, "cos") == 0)
                push(&p, cos(grausParaRad(a)));
            else if (strcmp(token, "tg") == 0)
                push(&p, tan(grausParaRad(a)));
            else if (strcmp(token, "log") == 0)
                push(&p, log10(a));
            else if (strcmp(token, "raiz") == 0)
                push(&p, sqrt(a));
        } else {
            float b = pop(&p);
            float a = pop(&p);
            if (strcmp(token, "+") == 0)
                push(&p, a + b);
            else if (strcmp(token, "-") == 0)
                push(&p, a - b);
            else if (strcmp(token, "*") == 0)
                push(&p, a * b);
            else if (strcmp(token, "/") == 0)
                push(&p, a / b);
            else if (strcmp(token, "%") == 0)
                push(&p, fmod(a, b));
            else if (strcmp(token, "^") == 0)
                push(&p, pow(a, b));
        }
        token = strtok(NULL, " ");
    }
    return pop(&p);
}

float getValorInFixa(char *StrInFixa) {
    char copia[MAX];
    strcpy(copia, getFormaPosFixa(StrInFixa));
    return getValorPosFixa(copia);
}
