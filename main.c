// Vinícius de Castro Duarte - 24.123.073-9
// Renan Ivaskevicius Vieira - 24.123.069-7

#include <stdio.h>
#include "functions.h"

int main() {
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
                    novo_cliente();
                    printf("Cliente Cadastrado!\n");
                }
                break;
            case 2:
                if (confirmacao(2) == 'S') {
                    printf("Opcao 2 confirmada!\n");
                    apagar_cpf();
                }
                break;
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
                }
                break;
            case 5:
                if (confirmacao(5) == 'S') {
                    printf("Opcao 5 confirmada!\n");
                    deposito();
                }
                break;
            case 6:
                if (confirmacao(6) == 'S') {
                    printf("Opção 6 confirmada!\n");
                    extrato(input_cpf());
                    printf("Extrato bancário:\n");
                }
                break;
            case 7:
                if (confirmacao(7) == 'S') {
                    printf("Opção 7 confirmada!\n");
                    FILE* arquivo = fopen("clients.bin", "rb+");

                    if (arquivo == NULL) {
                        printf("Erro ao abrir o arquivo.\n");
                        return 1;
                    }

                    char cnpj_origem[15], cnpj_destino[15];
                    double valor;

                    printf("Digite o CNPJ de origem: ");
                    scanf("%s", cnpj_origem);

                    printf("Digite o CNPJ de destino: ");
                    scanf("%s", cnpj_destino);

                    printf("Digite o valor a ser transferido: R$");
                    scanf("%lf", &valor);

                    realizarTransferencia(arquivo, cnpj_origem, cnpj_destino, valor);

                    fclose(arquivo);
                }
                break;
            case 8:
                if (confirmacao(8) == 'S') {
                    printf("Opcao 8 confirmada!\n");
                    debito_automatico();
                }
                break;
            case 9:
                printf("Volte Sempre!\n");
                return 0;
        }
    }
}
