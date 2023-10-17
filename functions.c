// Vinícius de Castro Duarte - 24.123.073-9
// Renan Ivaskevicius Vieira - 24.123.069-7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "functions.h"

typedef struct {
    char razao_social[100];
    char cnpj[15];
    char tipo_conta;
    double saldo;
    char senha[50];
    char transacoes[1000][100];
    int num_transacoes;
} Cliente;

Cliente clientes[100];
int numClientes = 0;

// Confirma se a opcao desejada esta de acordo
char confirmacao(int operacao) {
    char confirmacao;
    printf("\033[1;33m Você escolheu a opção %d, confirma? [S/N] ", operacao);
    scanf(" %c", &confirmacao);
    return toupper(confirmacao);
}

// Confirma o que foi selecionado
char confirma_input() {
    char confirma;
    printf("Confirma? [S/N] ");
    scanf(" %c", &confirma);
    return toupper(confirma);
}

// Recolhe o cnpj do usuario
char* input_cnpj() {
    static char cnpj[15];
    printf("Digite seu CNPJ: ");
    scanf(" %s", cnpj);

    // Remover pontuações do CNPJ
    int i, j = 0;
    for (i = 0; cnpj[i]; i++) {
        if (isdigit(cnpj[i])) {
            cnpj[j++] = cnpj[i];
        }
    }
    cnpj[j] = '\0';

    return cnpj;
}

// opcao 1 - novo cliente
void novo_cliente() {
    Cliente novo;
    memset(&novo, 0, sizeof(Cliente));

    printf("Digite a razao social: ");
    scanf(" %s", novo.razao_social);

    while (confirma_input() != 'S') {
        printf("Digite a razao social: ");
        scanf(" %s", novo.razao_social);
    }

    char* cnpj = input_cnpj();
    while (confirma_input() != 'S') {
        cnpj = input_cnpj();
    }

    printf("Tipo de conta (Comum/Plus): ");
    scanf(" %c", &novo.tipo_conta);

    while (novo.tipo_conta != 'C' && novo.tipo_conta != 'P') {
        printf("Escolha o seu tipo de conta, Comum ou Plus: ");
        scanf(" %c", &novo.tipo_conta);
    }

    printf("Valor inicial da conta: R$");
    scanf("%lf", &novo.saldo);

    while (confirma_input() != 'S') {
        printf("Valor inicial da conta: R$");
        scanf("%lf", &novo.saldo);
    }

    printf("Senha do usuario: ");
    scanf(" %s", novo.senha);

    char confirma_senha[50];
    printf("Confirme sua senha: ");
    scanf(" %s", confirma_senha);

    while (strcmp(novo.senha, confirma_senha) != 0) {
        printf("Confirme sua senha da forma correta: ");
        scanf(" %s", confirma_senha);
    }

    strcpy(clientes[numClientes].razao_social, novo.razao_social);
    strcpy(clientes[numClientes].cnpj, cnpj);
    clientes[numClientes].tipo_conta = novo.tipo_conta;
    clientes[numClientes].saldo = novo.saldo;
    strcpy(clientes[numClientes].senha, novo.senha);

    FILE *file = fopen("clients.txt", "a"); // O modo "a" indica que estamos abrindo o arquivo para adicionar (append) informações

    // Verifica se o arquivo foi aberto com sucesso
    if (file != NULL) {
        // Escreve as informações do cliente no arquivo
        fprintf(file, "Razão Social: %s\n", clientes[numClientes].razao_social);
        fprintf(file, "CNPJ: %s\n", clientes[numClientes].cnpj);
        fprintf(file, "Tipo de Conta: %d\n", clientes[numClientes].tipo_conta);
        fprintf(file, "Saldo: %.2lf\n", clientes[numClientes].saldo);
        fprintf(file, "Senha: %s\n\n", clientes[numClientes].senha);

        // Fecha o arquivo
        fclose(file);
    }

    numClientes++;
}

// opcao 3 - listar clientes
void listar() {
    FILE *file = fopen("clients.txt", "r");

    if (file == NULL) {
        printf("Nenhum cliente cadastrado.\n");
    } else {
        Cliente cliente;
        char temp[100]; // Usaremos temp para ler os rótulos (por exemplo, "Razão Social:")

        while (fscanf(file, "%*[^:]: %99[^\n]\n%*[^:]: %14[^\n]\n%*[^:]: %c\n%*[^:]: %lf\n%*[^:]: %49[^\n]\n",
                      cliente.razao_social, cliente.cnpj, &cliente.tipo_conta, &cliente.saldo, cliente.senha) == 5) {
            printf("-=-=-=-=-=-=-=-=-=-=-=-\n");
            printf("CNPJ: %s\n", cliente.cnpj);
            printf("Razão Social: %s\n", cliente.razao_social);
            printf("Tipo de conta: %c\n", cliente.tipo_conta);
            printf("Valor em conta: R$%.2lf\n", cliente.saldo);
            printf("Senha: %s\n", cliente.senha);
            printf("\n");
        }

        fclose(file);
    }
}
