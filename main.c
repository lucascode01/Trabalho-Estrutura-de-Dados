#include <stdio.h>
#include <string.h>
#include "expressao.h"

int main() {
    Expressao exp;
    int tipo;

    printf("Tipo da expressao? (1 = infixa, 2 = pos-fixa): ");
    scanf("%d", &tipo);
    getchar(); 

    if (tipo == 1) {
        printf("Digite a expressao infixa: ");
        fgets(exp.inFixa, 512, stdin);
        exp.inFixa[strcspn(exp.inFixa, "\n")] = '\0';

        strcpy(exp.posFixa, getFormaPosFixa(exp.inFixa));
        exp.Valor = getValorInFixa(exp.inFixa);

        printf("\nForma Pos-Fixa: %s\n", exp.posFixa);
        printf("Valor da expressao: %.5f\n", exp.Valor);

    } else if (tipo == 2) {
        printf("Digite a expressao pos-fixa: ");
        fgets(exp.posFixa, 512, stdin);
        exp.posFixa[strcspn(exp.posFixa, "\n")] = '\0';

        strcpy(exp.inFixa, getFormaInFixa(exp.posFixa));
        exp.Valor = getValorPosFixa(exp.posFixa);

        printf("\nForma Infixa (simulada): %s\n", exp.inFixa);
        printf("Valor da expressao: %.5f\n", exp.Valor);

    } else {
        printf("\nTipo inválido.\n");
    }

    return 0;
}
