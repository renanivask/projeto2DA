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
            printf("Digite um numero entre 1 e 9! Tente novamente!\n");
            continue;
        }

        switch (operacao) {
            case 1:
                if (confirmacao(1) == 'S') {
                    printf("Opcao 1 confirmada!\n");
                    novo_cliente();
                    printf("Cliente Cadastrado!\n");
                }
                break;
            // case 2:
            //     if (confirmacao(2) == 'S') {
            //         printf("Opcao 2 confirmada!\n");
            //         printf("Digite o CNPJ a ser removido: ");
            //         scanf("%d", &cnpj_apagar);
            //         deletar(&cnpj_apagar);
            //     }
            //     break;
            case 3:
                if (confirmacao(3) == 'S') {
                    printf("Opcao 3 confirmada!\n");
                    printf("Lista de clientes:\n");
                    listar();
                }
                break;
            case 4:
                if (confirmacao(4) == 'S') {
                    printf("Opcao 4 confirmada!\n");
                    debito();
                    printf("Operacao de debito realizada!\n");
                }
                break;
            case 5:
                if (confirmacao(5) == 'S') {
                    printf("Opcao 5 confirmada!\n");
                    deposito();
                }
                break;
            case 8:
                if (confirmacao(8) == 'S') {
                    printf("Opcao 8 confirmada!\n");
                    debito_automatico();
                    printf("Debito automatico configurado!\n");
                }
                break;
            case 9:
                printf("Volte Sempre!\n");
                return 0;
        }
    }
}
