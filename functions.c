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
