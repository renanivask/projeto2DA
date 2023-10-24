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

// Opcao 1 - novo cliente
void novo_cliente() {
    Cliente novo;
    memset(&novo, 0, sizeof(Cliente));

    printf("Digite o CNPJ (APENAS NUMEROS): ");
    scanf(" %s", novo.cnpj);

    printf("Digite a razão social: ");
    scanf(" %s", novo.razao_social);

    printf("Tipo de conta [C / P]: ");
    scanf(" %c", &novo.tipo_conta);

    while (novo.tipo_conta != 'C' && novo.tipo_conta != 'P') {
        printf("Escolha o seu tipo de conta, Comum ou Plus: ");
        scanf(" %c", &novo.tipo_conta);
    }

    printf("Valor inicial da conta: R$");
    scanf("%lf", &novo.saldo);

    printf("Senha do usuário: ");
    scanf(" %s", novo.senha);

    char confirma_senha[50];
    printf("Confirme sua senha: ");
    scanf(" %s", confirma_senha);

    while (strcmp(novo.senha, confirma_senha) != 0) {
        printf("Confirme sua senha da forma correta: ");
        scanf(" %s", confirma_senha);
    }

    // Abra um arquivo binário para escrita
    FILE *file = fopen("clients.bin", "ab");

    // Verifique se o arquivo foi aberto com sucesso
    if (file != NULL) {
        // Escreva as informações do cliente no arquivo
        fwrite(&novo, sizeof(Cliente), 1, file);

        // Feche o arquivo
        fclose(file);

        printf("Cliente cadastrado com sucesso!\n");
    } else {
        printf("Erro ao abrir o arquivo binário para escrita.\n");
    }
}

// Opcao 3 - listar clientes
void listar() {
    // Abra o arquivo binário para leitura
    FILE *file = fopen("clients.bin", "rb");

    if (file == NULL) {
        printf("Nenhum cliente cadastrado.\n");
    } else {
        Cliente cliente;

        while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
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

// Operacao 4: Debito
void debito() {
    char cnpj[15];
    printf("Digite o CNPJ do cliente: ");
    scanf("%s", cnpj);

    FILE* file = fopen("clients.bin", "rb+"); // Abra o arquivo em modo de leitura e escrita binária.

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
        if (strcmp(cliente.cnpj, cnpj) == 0) {
            char senha[50];
            printf("Digite sua senha: ");
            scanf(" %s", senha);

            while (strcmp(cliente.senha, senha) != 0) {
                printf("Senha incorreta. Digite novamente sua senha: ");
                scanf(" %s", senha);
            }

            double valor_debito;
            printf("Qual valor você deseja debitar: R$");
            scanf("%lf", &valor_debito);

            double valor = valor_debito;
            double taxa = 0.0;

            if (cliente.tipo_conta == 'C') {
                taxa = valor * 0.05;
                double novo_saldo = cliente.saldo - (valor + taxa);

                while (novo_saldo < -1000.0) {
                    printf("Limite de R$1000.00 atingido.\n");
                    printf("Digite um valor valido: R$");
                    scanf("%lf", &valor_debito);
                    valor = valor_debito;
                    novo_saldo = cliente.saldo - (valor + taxa);
                }

                cliente.saldo = novo_saldo;
            } else {
                taxa = valor * 0.03;
                double novo_saldo = cliente.saldo - (valor + taxa);

                while (novo_saldo < -5000.0) {
                    printf("Limite de R$5000.00 atingido.\n");
                    printf("Digite um valor valido: R$");
                    scanf("%lf", &valor_debito);
                    valor = valor_debito;
                    novo_saldo = cliente.saldo - (valor + taxa);
                }

                cliente.saldo = novo_saldo;
            }

            // Atualize as transações do cliente
            time_t current_time;
            time(&current_time);
            struct tm* local_time = localtime(&current_time);

            sprintf(cliente.transacoes[cliente.num_transacoes], "Data: %04d-%02d-%02d %02d:%02d:%02d / -R$%.2lf / Tarifa: R$%.2lf / Saldo: R$%.2lf",
                    local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
                    local_time->tm_hour, local_time->tm_min, local_time->tm_sec, valor, taxa, cliente.saldo);

            // Atualize o número de transações
            cliente.num_transacoes++;

            // Volte para a posição correta no arquivo
            fseek(file, -sizeof(Cliente), SEEK_CUR);

            // Escreva o registro atualizado de volta no arquivo binário
            fwrite(&cliente, sizeof(Cliente), 1, file);

            printf("Saldo de R$%.2lf\n", cliente.saldo);
            fclose(file);
            return;
        }
    }

    printf("Cliente nao encontrado!\n");
    fclose(file);
}

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
    printf("\033[1;33m Voce escolheu a opcao %d, confirma? [S/N] ", operacao);
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

// Opcao 1 - novo cliente
void novo_cliente() {
    Cliente novo;
    memset(&novo, 0, sizeof(Cliente));

    printf("Digite o CNPJ (APENAS NUMEROS): ");
    scanf(" %s", novo.cnpj);

    printf("Digite a razão social: ");
    scanf(" %s", novo.razao_social);

    printf("Tipo de conta [C / P]: ");
    scanf(" %c", &novo.tipo_conta);

    while (novo.tipo_conta != 'C' && novo.tipo_conta != 'P') {
        printf("Escolha o seu tipo de conta, Comum ou Plus: ");
        scanf(" %c", &novo.tipo_conta);
    }

    printf("Valor inicial da conta: R$");
    scanf("%lf", &novo.saldo);

    printf("Senha do usuário: ");
    scanf(" %s", novo.senha);

    char confirma_senha[50];
    printf("Confirme sua senha: ");
    scanf(" %s", confirma_senha);

    while (strcmp(novo.senha, confirma_senha) != 0) {
        printf("Confirme sua senha da forma correta: ");
        scanf(" %s", confirma_senha);
    }

    // Abra um arquivo binário para escrita
    FILE *file = fopen("clients.bin", "ab");

    // Verifique se o arquivo foi aberto com sucesso
    if (file != NULL) {
        // Escreva as informações do cliente no arquivo
        fwrite(&novo, sizeof(Cliente), 1, file);

        // Feche o arquivo
        fclose(file);

        printf("Cliente cadastrado com sucesso!\n");
    } else {
        printf("Erro ao abrir o arquivo binário para escrita.\n");
    }
}

// Opcao 3 - listar clientes
void listar() {
    // Abra o arquivo binário para leitura
    FILE *file = fopen("clients.bin", "rb");

    if (file == NULL) {
        printf("Nenhum cliente cadastrado.\n");
    } else {
        Cliente cliente;

        while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
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

// Operacao 4: Debito
void debito() {
    char cnpj[15];
    printf("Digite o CNPJ do cliente: ");
    scanf("%s", cnpj);

    FILE* file = fopen("clients.bin", "rb+"); // Abra o arquivo em modo de leitura e escrita binária.

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
        if (strcmp(cliente.cnpj, cnpj) == 0) {
            char senha[50];
            printf("Digite sua senha: ");
            scanf(" %s", senha);

            while (strcmp(cliente.senha, senha) != 0) {
                printf("Senha incorreta. Digite novamente sua senha: ");
                scanf(" %s", senha);
            }

            double valor_debito;
            printf("Qual valor você deseja debitar: R$");
            scanf("%lf", &valor_debito);

            double valor = valor_debito;
            double taxa = 0.0;

            if (cliente.tipo_conta == 'C') {
                taxa = valor * 0.05;
                double novo_saldo = cliente.saldo - (valor + taxa);

                while (novo_saldo < -1000.0) {
                    printf("Limite de R$1000.00 atingido.\n");
                    printf("Digite um valor valido: R$");
                    scanf("%lf", &valor_debito);
                    valor = valor_debito;
                    novo_saldo = cliente.saldo - (valor + taxa);
                }

                cliente.saldo = novo_saldo;
            } else {
                taxa = valor * 0.03;
                double novo_saldo = cliente.saldo - (valor + taxa);

                while (novo_saldo < -5000.0) {
                    printf("Limite de R$5000.00 atingido.\n");
                    printf("Digite um valor valido: R$");
                    scanf("%lf", &valor_debito);
                    valor = valor_debito;
                    novo_saldo = cliente.saldo - (valor + taxa);
                }

                cliente.saldo = novo_saldo;
            }

            // Atualize as transações do cliente
            time_t current_time;
            time(&current_time);
            struct tm* local_time = localtime(&current_time);

            sprintf(cliente.transacoes[cliente.num_transacoes], "Data: %04d-%02d-%02d %02d:%02d:%02d / -R$%.2lf / Tarifa: R$%.2lf / Saldo: R$%.2lf",
                    local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
                    local_time->tm_hour, local_time->tm_min, local_time->tm_sec, valor, taxa, cliente.saldo);

            // Atualize o número de transações
            cliente.num_transacoes++;

            // Volte para a posição correta no arquivo
            fseek(file, -sizeof(Cliente), SEEK_CUR);

            // Escreva o registro atualizado de volta no arquivo binário
            fwrite(&cliente, sizeof(Cliente), 1, file);

            printf("Saldo de R$%.2lf\n", cliente.saldo);
            fclose(file);
            return;
        }
    }

    printf("Cliente nao encontrado!\n");
    fclose(file);
}

void deposito() {
    char cnpj[15];
    printf("Digite o CNPJ do cliente: ");
    scanf("%s", cnpj);

    FILE* file = fopen("clients.bin", "rb+"); // Abra o arquivo em modo de leitura e escrita binária.

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
        if (strcmp(cliente.cnpj, cnpj) == 0) {
            char senha[50];
            printf("Digite sua senha: ");
            scanf(" %s", senha);

            while (strcmp(cliente.senha, senha) != 0) {
                printf("Senha incorreta. Digite novamente sua senha: ");
                scanf(" %s", senha);
            }

            double valor_credito;
            printf("Qual valor você deseja creditar: R$");
            scanf("%lf", &valor_credito);

            double valor = valor_credito;
            double taxa = 0.0;

            if (cliente.tipo_conta == 'C') {
                double novo_saldo = cliente.saldo + (valor + taxa);

                while (novo_saldo < -1000.0) {
                    printf("Limite de R$1000.00 atingido.\n");
                    printf("Digite um valor valido: R$");
                    scanf("%lf", &valor_credito);
                    valor = valor_credito;
                    novo_saldo = cliente.saldo - (valor + taxa);
                }

                cliente.saldo = novo_saldo;
            } else {
                double novo_saldo = cliente.saldo + (valor + taxa);

                while (novo_saldo < -5000.0) {
                    printf("Limite de R$5000.00 atingido.\n");
                    printf("Digite um valor valido: R$");
                    scanf("%lf", &valor_credito);
                    valor = valor_credito;
                    novo_saldo = cliente.saldo + (valor + taxa);
                }

                cliente.saldo = novo_saldo;
            }

            // Atualize as transações do cliente
            time_t current_time;
            time(&current_time);
            struct tm* local_time = localtime(&current_time);

            sprintf(cliente.transacoes[cliente.num_transacoes], "Data: %04d-%02d-%02d %02d:%02d:%02d / +R$%.2lf / Tarifa: R$%.2lf / Saldo: R$%.2lf",
                    local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
                    local_time->tm_hour, local_time->tm_min, local_time->tm_sec, valor, taxa, cliente.saldo);

            // Atualize o número de transações
            cliente.num_transacoes++;

            // Volte para a posição correta no arquivo
            fseek(file, -sizeof(Cliente), SEEK_CUR);

            // Escreva o registro atualizado de volta no arquivo binário
            fwrite(&cliente, sizeof(Cliente), 1, file);

            printf("Saldo de R$%.2lf\n", cliente.saldo);
            fclose(file);
            return;
        }
    }

    printf("Cliente nao encontrado!\n");
    fclose(file);
}

// Operacao 8: Débito Automático
void debito_automatico() {
    char cnpj[15];
    printf("Digite o CNPJ do cliente: ");
    scanf("%s", cnpj);

    FILE* file = fopen("clients.bin", "rb+"); // Abra o arquivo em modo de leitura e escrita binária.

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
        if (strcmp(cliente.cnpj, cnpj) == 0) {
            char senha[50];
            printf("Digite sua senha: ");
            scanf(" %s", senha);

            while (strcmp(cliente.senha, senha) != 0) {
                printf("Senha incorreta. Digite novamente sua senha: ");
                scanf(" %s", senha);
            }

            double valor_debito;
            printf("Qual o valor do debito automatico: R$");
            scanf("%lf", &valor_debito);

            int dia;
            printf("Em qual dia do mes você deseja o debito automatico: ");
            scanf("%d", &dia);

            if (dia < 1 || dia > 31) {
                printf("Dia invalido. O dia deve estar entre 1 e 31.\n");
                fclose(file);
                return;
            }

            double valor = valor_debito;
            double novo_saldo = cliente.saldo - valor;

            while (novo_saldo < -1000.0) {
                printf("Limite de R$1000.00 atingido.\n");
                printf("Digite um valor valido: R$");
                scanf("%lf", &valor_debito);
                valor = valor_debito;
                novo_saldo = cliente.saldo - valor;
            }

            // Obtenha a data e hora atual
            time_t current_time;
            time(&current_time);
            struct tm* local_time = localtime(&current_time);

            // Agende o débito automático
            sprintf(cliente.transacoes[cliente.num_transacoes], "Debito automatico agendado para o dia %d/%02d/%04d / -R$%.2lf / Tarifa: R$0.00 / Saldo: R$%.2lf",
                    dia, local_time->tm_mon + 1, local_time->tm_year + 1900, valor, novo_saldo);

            cliente.num_transacoes++;
            cliente.saldo = novo_saldo;

            // Volte para a posição correta no arquivo
            fseek(file, -sizeof(Cliente), SEEK_CUR);

            // Escreva o registro atualizado de volta no arquivo binário
            fwrite(&cliente, sizeof(Cliente), 1, file);

            printf("Debito automatico agendado com sucesso!\n");
            fclose(file);
            return;
        }
    }

    printf("Cliente nao encontrado!\n");
    fclose(file);
}
