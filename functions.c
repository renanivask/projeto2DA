// Vinícius de Castro Duarte - 24.123.073-9
// Renan Ivaskevicius Vieira - 24.123.069-7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "functions.h"

// Confirma se a opcao desejada esta de acordo
char confirmacao(int operacao) {
    char confirmacao;
    printf("\033[1;32m Voce escolheu a opcao %d, confirma? [S/N] ", operacao);
    scanf(" %c", &confirmacao);
    return toupper(confirmacao);
}

int confirma_abertura_arquivo(char arquivo[30]){
  FILE *file = fopen(arquivo, "r");

  if(file != NULL){
    return 1;
  }else{
    return 0;
  }
};

int arquivo_escrito(char arquivo[30]){
  FILE *file = fopen(arquivo, "r");

  fseek(file, 0, SEEK_END);

  long tamanho = ftell(file);

  if(tamanho > 0){
    fclose(file);
    return 1;
  }else{
    fclose(file);
    return 0;
  }
};

// Opcao 1 - novo cliente
void novo_cliente(Cliente* clientes, int* numClientes) {
    Cliente novo;
    memset(&novo, 0, sizeof(Cliente));

    printf("Digite o cpf (APENAS NUMEROS): ");
    scanf(" %s", novo.cpf);

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

  FILE *file = fopen("clients.bin", "ab");

  // Verifica se o arquivo foi aberto com sucesso
  if (confirma_abertura_arquivo("clients.bin") == 1) {
      // Escreva as informações do cliente no arquivo
      fwrite(&novo, sizeof(Cliente), 1, file);

      // Fecha o arquivo
      fclose(file);

      // Verifica se o número de clientes está abaixo do limite
      if (*numClientes < 100) {
          // Adiciona o novo cliente ao array de clientes
          clientes[*numClientes] = novo;
          (*numClientes)++;
          printf("Cliente cadastrado com sucesso!\n");
      } else {
          printf("Limite máximo de clientes atingido!\n");
      }
  } else {
      printf("Erro ao abrir o arquivo para escrita!\n");
  }
}

void apagar_cpf(Cliente* clientes, int* numClientes) {
    char cpf[15];
    char senha[50];

    printf("Digite seu cpf: ");
    scanf(" %s", cpf);
    printf("Digite sua senha: ");
    scanf(" %s", senha);

    FILE *file = fopen("clients.bin", "rb");

    if (arquivo_escrito("clients.bin") == 0) {
        printf("Não existem clientes cadastrados.\n");
        return;
    }

    FILE *tempFile = fopen("temp_clients.bin", "wb");

    if (confirma_abertura_arquivo("temp_clients.bin") == 0) {
        printf("ERRO!\n");
        fclose(file);
        return;
    }

    Cliente cliente;

    int clienteRemovido = 0;  // Flag para indicar se o cliente foi removido

    while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
        // Verifica se o cpf e a senha correspondem
        if (strcmp(cliente.cpf, cpf) == 0 && strcmp(cliente.senha, senha) == 0) {
            printf("Cliente apagado com sucesso!\n");
            clienteRemovido = 1;
            continue; // Não escreve este cliente no arquivo temporário
        }

        // Escreve os outros clientes no arquivo temporário
        fwrite(&cliente, sizeof(Cliente), 1, tempFile);
    }

    fclose(file);
    fclose(tempFile);

    // Renomeia o arquivo temporário para o arquivo original
    if (remove("clients.bin") == 0 && rename("temp_clients.bin", "clients.bin") == 0) {
        if (clienteRemovido) {
            // Se o cliente foi removido, atualiza o número de clientes
            (*numClientes)--;
        }
    }
}

// Opcao 3 - listar clientes
void listar(void) {
    // Abre o arquivo binário para leitura
    FILE *file = fopen("clients.bin", "rb");

    if (arquivo_escrito("clients.bin") == 0) {
        printf("Nenhum usuario cadastrado.\n");
    } else {
        Cliente cliente;

        while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
            printf("-=-=-=-=-=-=-=-=-=-=-=-\n");
            printf("Cpf: %s\n", cliente.cpf);
            printf("Tipo de conta: %c\n", cliente.tipo_conta);
            printf("Valor em conta: R$%.2lf\n", cliente.saldo);
            printf("Senha: %s\n", cliente.senha);
            printf("\n");
        }

        fclose(file);
    }
}

// Função para realizar um débito na conta de um cliente
void debito(Cliente* clientes, int* numClientes) {
    char cpf[15];
    printf("Digite o CPF do cliente: ");
    scanf("%s", cpf);

    FILE* file = fopen("clients.bin", "rb+");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Cliente cliente;

    int clienteEncontrado = 0;

    while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
        if (strcmp(cliente.cpf, cpf) == 0) {
            clienteEncontrado = 1;
            break;
        }
    }

    if (!clienteEncontrado) {
        printf("Cliente não encontrado!\n");
        fclose(file);
        return;
    }

    char senha[50];
    printf("Digite sua senha: ");
    scanf(" %s", senha);

    // Verifica a senha
    if (strcmp(cliente.senha, senha) != 0) {
        printf("Senha incorreta!\n");
        fclose(file);
        return;
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
            printf("Digite um valor válido: R$");
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
            printf("Digite um valor válido: R$");
            scanf("%lf", &valor_debito);
            valor = valor_debito;
            novo_saldo = cliente.saldo - (valor + taxa);
        }

        cliente.saldo = novo_saldo;
    }

    // Atualiza as transações do cliente
    time_t tempoAtual;
    time(&tempoAtual);
    struct tm* tempo_local = localtime(&tempoAtual);

    sprintf(cliente.transacoes[cliente.num_transacoes], "Data: %04d-%02d-%02d %02d:%02d:%02d / -R$%.2lf / Tarifa: R$%.2lf / Saldo: R$%.2lf",
            tempo_local->tm_year + 1900, tempo_local->tm_mon + 1, tempo_local->tm_mday,
            tempo_local->tm_hour, tempo_local->tm_min, tempo_local->tm_sec, valor, taxa, cliente.saldo);

    // Atualiza o número de transações
    cliente.num_transacoes++;

    // Volta para a posição correta no arquivo
    fseek(file, -sizeof(Cliente), SEEK_CUR);

    // Escreve o registro atualizado de volta no arquivo binário
    fwrite(&cliente, sizeof(Cliente), 1, file);

    printf("Saldo de R$%.2lf\n", cliente.saldo);
    fclose(file);
}


// Função para realizar um depósito na conta de um cliente
void deposito(Cliente* clientes, int* numClientes) {
    char cpf[15];
    printf("Digite o cpf do cliente: ");
    scanf("%s", cpf);

    FILE* file = fopen("clients.bin", "rb+"); // Abre o arquivo em modo de leitura e escrita binária.

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
        if (strcmp(cliente.cpf, cpf) == 0) {
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
                    printf("Digite um valor válido: R$");
                    scanf("%lf", &valor_credito);
                    valor = valor_credito;
                    novo_saldo = cliente.saldo - (valor + taxa);
                }

                cliente.saldo = novo_saldo;
            } else {
                double novo_saldo = cliente.saldo + (valor + taxa);

                while (novo_saldo < -5000.0) {
                    printf("Limite de R$5000.00 atingido.\n");
                    printf("Digite um valor válido: R$");
                    scanf("%lf", &valor_credito);
                    valor = valor_credito;
                    novo_saldo = cliente.saldo + (valor + taxa);
                }

                cliente.saldo = novo_saldo;
            }

            // Atualiza as transações do cliente
            time_t tempoAtual;
            time(&tempoAtual);
            struct tm* tempo_local = localtime(&tempoAtual);

            sprintf(cliente.transacoes[cliente.num_transacoes], "Data: %04d-%02d-%02d %02d:%02d:%02d / +R$%.2lf / Tarifa: R$%.2lf / Saldo: R$%.2lf",
                    tempo_local->tm_year + 1900, tempo_local->tm_mon + 1, tempo_local->tm_mday,
                    tempo_local->tm_hour, tempo_local->tm_min, tempo_local->tm_sec, valor, taxa, cliente.saldo);

            // Atualiza o número de transações
            cliente.num_transacoes++;

            // Volta para a posição correta no arquivo
            fseek(file, -sizeof(Cliente), SEEK_CUR);

            // Escreve o registro atualizado de volta no arquivo binário
            fwrite(&cliente, sizeof(Cliente), 1, file);

            printf("Saldo de R$%.2lf\n", cliente.saldo);
            fclose(file);
            return;
        }
    }

    printf("Cliente não encontrado no arquivo!\n");
    fclose(file);
}
