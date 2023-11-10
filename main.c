// Vinícius de Castro Duarte - 24.123.073-9
// Renan Ivaskevicius Vieira - 24.123.069-7

#include <stdio.h>
#include "functions.h"

int main() {

    Cliente clientes[50];
    int numClientes = 0;

    int operacao;

    while (1) {
        printf("\n\033[1;37mBem vindo ao banco QuemPoupaTem! Qual operacao voce gostaria de realizar?\n");
        printf("\033[1;31m[ 1 ] - Novo cliente.\n");
        printf("\033[1;33m[ 2 ] - Apaga cliente.\n");
        printf("\033[1;32m[ 3 ] - Listar clientes.\n");
        printf("\033[1;36m[ 4 ] - Debito.\n");
        printf("\033[1;34m[ 5 ] - Deposito.\n");
        printf("\033[1;35m[ 6 ] - Extrato.\n");
        printf("\033[1;31m[ 7 ] - Transferencia entre contas.\n");
        printf("\033[1;33m[ 8 ] - Debito automatico.\n");
        printf("\033[1;32m[ 9 ] - Sair.\n");

        scanf("%d", &operacao);

        if (operacao < 1 || operacao > 9) {
            printf("Digite um numero entre 1 e 9! Tente novamente!\n");
            continue;
        }

        switch (operacao) {
            case 1:
                if (confirmacao(1) == 'S') {
                    printf("Opcao 1 confirmada!\n");
                    novo_cliente(clientes, &numClientes);
                }
                break;
            case 2:
                if (confirmacao(2) == 'S') {
                    printf("Opcao 2 confirmada!\n");
                    apagar_cpf(clientes, &numClientes);
                }
                break;

    }
  }
}
