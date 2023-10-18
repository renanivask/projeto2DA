// Vinícius de Castro Duarte - 24.123.073-9
// Renan Ivaskevicius Vieira - 24.123.069-7

#include <stdio.h>
#include "functions.h"

int main() {
    int operacao;
    int cnpj_apagar;

    while (1) {
        printf("\nBem vindo ao banco QuemPoupaTem! Qual operacao voce gostaria de realizar?\n");
        printf("[ 1 ] - Novo cliente.\n");
        printf("[ 2 ] - Apaga cliente.\n");
        printf("[ 3 ] - Listar clientes.\n");
        printf("[ 4 ] - Debito.\n");
        printf("[ 5 ] - Deposito.\n");
        printf("[ 6 ] - Extrato.\n");
        printf("[ 7 ] - Transferencia entre contas.\n");
        printf("[ 8 ] - Debito automatico.\n");
        printf("[ 9 ] - Sair.\n");

        scanf("%d", &operacao);

        if (operacao < 1 || operacao > 9) {
            printf("Digite um número entre 1 e 9! Tente novamente!\n");
            continue;
        }

        switch (operacao) {
            case 1:
                if (confirmacao(1) == 'S') {
                    printf("Opção 1 confirmada!\n");
                    novo_cliente();
                    printf("Cliente Cadastrado!\n");
                }
                break;
            case 2:
                if (confirmacao(2) == 'S') {
                    printf("Opcao 2 confirmada!\n");
                    printf("Digite o CNPJ a ser removido: ");
                    scanf("%d", &cnpj_apagar);
                    deletar(&cnpj_apagar);
                }
                break;
            case 3:
                if (confirmacao(3) == 'S') {
                    printf("Opção 3 confirmada!\n");
                    listar();
                    printf("Lista de clientes:\n");
                }
                break;
              case 9:
                printf("Volte Sempre!\n");
                return 0;
        }
    }
}
