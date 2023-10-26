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
    char cpf[15];
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

// Recolhe o cpf do usuario
char* input_cpf() {
    static char cpf[15];
    printf("Digite seu cpf: ");
    scanf(" %s", cpf);

    // Remove pontuações do cpf
    int i, j = 0;
    for (i = 0; cpf[i]; i++) {
        if (isdigit(cpf[i])) {
            cpf[j++] = cpf[i];
        }
    }
    cpf[j] = '\0';

    return cpf;
}

// Opcao 1 - novo cliente
void novo_cliente() {
    Cliente novo;
    memset(&novo, 0, sizeof(Cliente));

    printf("Digite o cpf (APENAS NUMEROS): ");
    scanf(" %s", novo.cpf);

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

    // Abre um arquivo binário para escrita
    FILE *file = fopen("clients.bin", "ab");

    // Verifica se o arquivo foi aberto com sucesso
    if (file != NULL) {
        // Escreva as informações do cliente no arquivo
        fwrite(&novo, sizeof(Cliente), 1, file);

        // Fecha o arquivo
        fclose(file);

        printf("Cliente cadastrado com sucesso!\n");
    } else {
        printf("Erro ao abrir o arquivo binário para escrita.\n");
    }
}

void apagar_cpf() {
    char cpf[15];
    char senha[50];

    printf("Digite seu cpf: ");
    scanf(" %s", cpf);
    printf("Digite sua senha: ");
    scanf(" %s", senha);

    FILE *file = fopen("clients.bin", "rb");

    if (file == NULL) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    FILE *tempFile = fopen("temp_clients.bin", "wb");

    if (tempFile == NULL) {
        printf("Erro ao abrir o arquivo temporário.\n");
        fclose(file);
        return;
    }

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
        // Verifica se o cpf e a senha correspondem
        if (strcmp(cliente.cpf, cpf) == 0 && strcmp(cliente.senha, senha) == 0) {
            printf("Cliente apagado com sucesso!\n");
            continue; // Não escreve este cliente no arquivo temporário
        }

        // Escreve os outros clientes no arquivo temporário
        fwrite(&cliente, sizeof(Cliente), 1, tempFile);
    }

    fclose(file);
    fclose(tempFile);

    // Renomeia o arquivo temporário para o arquivo original
    if (remove("clients.bin") == 0 && rename("temp_clients.bin", "clients.bin") == 0) {
    }
}

// Opcao 3 - listar clientes
void listar() {
    // Abre o arquivo binário para leitura
    FILE *file = fopen("clients.bin", "rb");

    if (file == NULL) {
        printf("Nenhum cliente cadastrado.\n");
    } else {
        Cliente cliente;

        while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
            printf("-=-=-=-=-=-=-=-=-=-=-=-\n");
            printf("cpf: %s\n", cliente.cpf);
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

            // Atualiza as transações do cliente
            time_t current_time;
            time(&current_time);
            struct tm* local_time = localtime(&current_time);

            sprintf(cliente.transacoes[cliente.num_transacoes], "Data: %04d-%02d-%02d %02d:%02d:%02d / -R$%.2lf / Tarifa: R$%.2lf / Saldo: R$%.2lf",
                    local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
                    local_time->tm_hour, local_time->tm_min, local_time->tm_sec, valor, taxa, cliente.saldo);

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

    printf("Cliente nao encontrado!\n");
    fclose(file);
}

// Operacao 5: Depósito
void deposito() {
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

            // Atualiza as transações do cliente
            time_t current_time;
            time(&current_time);
            struct tm* local_time = localtime(&current_time);

            sprintf(cliente.transacoes[cliente.num_transacoes], "Data: %04d-%02d-%02d %02d:%02d:%02d / +R$%.2lf / Tarifa: R$%.2lf / Saldo: R$%.2lf",
                    local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
                    local_time->tm_hour, local_time->tm_min, local_time->tm_sec, valor, taxa, cliente.saldo);

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

    printf("Cliente nao encontrado!\n");
    fclose(file);
}

// Operacao 6: Extrato
void extrato(const char* cpf) {
    // Abre o arquivo binário para leitura
    FILE* file = fopen("clients.bin", "rb");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo binário.\n");
        return;
    }

    // Estrutura temporária para ler os clientes do arquivo
    Cliente cliente;

    // Localiza o cliente no arquivo binário
    int encontrado = 0;

    while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
        if (strcmp(cliente.cpf, cpf) == 0) {
            encontrado = 1;
            break;
        }
    }

    fclose(file);

    if (!encontrado) {
        printf("Cliente não encontrado.\n");
        return;
    }

    // Cria um arquivo de texto para o extrato
    char nome_arquivo[50];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "extrato_%s.txt", cpf);
    FILE* extrato_file = fopen(nome_arquivo, "w");

    if (extrato_file == NULL) {
        printf("Erro ao criar o arquivo de extrato.\n");
        return;
    }

    // Escreve o extrato no arquivo de texto
    fprintf(extrato_file, "Extrato para o cliente cpf: %s\n", cpf);

    // Adiciona o histórico de operações do cliente no extrato
    for (int i = 0; i < cliente.num_transacoes; i++) {
        fprintf(extrato_file, "%s\n", cliente.transacoes[i]);
    }

    fclose(extrato_file);
    printf("Extrato gerado com sucesso: %s\n", nome_arquivo);
}

// Início função 7 - Transferência de Contas
void transferencia_contas(){
  char cpf1[15], cpf2[15];
  printf("Digite o cpf do cliente que proverá o dinheiro: ");
  scanf("%s", cpf1);
  
  printf("Digite o cpf do cliente que receberá a quantia: ");
  scanf( "%s", cpf2);

      FILE* file = fopen("clients.bin", "rb+"); 

      if (file == NULL) {
          printf("Erro ao abrir o arquivo.\n");
          return;
      }

      Cliente cliente;

      while (fread(&cliente, sizeof(Cliente), 1, file) == 1) {
          if (strcmp(cliente.cpf, cpf1) == 0) {
              char senha[50];
              printf("Digite a senha da conta do cliente que proverá o dinheiro: ");
              scanf(" %s", senha);

              while (strcmp(cliente.senha, senha) != 0) {
                  printf("Senha incorreta. Digite novamente sua senha: ");
                  scanf(" %s", senha);
              }

              double valor_debito;
              printf("Qual valor você deseja transferir: R$");
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

              time_t current_time;
              time(&current_time);
              struct tm* local_time = localtime(&current_time);

              sprintf(cliente.transacoes[cliente.num_transacoes], "Data: %04d-%02d-%02d %02d:%02d:%02d / -R$%.2lf / Tarifa: R$%.2lf / Saldo: R$%.2lf",
                      local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
                      local_time->tm_hour, local_time->tm_min, local_time->tm_sec, valor, taxa, cliente.saldo);

              // Atualiza o número de transações
              cliente.num_transacoes++;

              // Volta para a posição correta no arquivo
              fseek(file, -sizeof(Cliente), SEEK_CUR);

              // Escreve o registro atualizado de volta no arquivo binário
              fwrite(&cliente, sizeof(Cliente), 1, file);

              printf("Saldo de R$%.2lf\n", cliente.saldo);
              fclose(file);
          }
      }

      FILE* file1 = fopen("clients.bin", "rb+");
    
      Cliente cliente1;
  
          while (fread(&cliente1, sizeof(Cliente), 1, file1) == 1) {
              if (strcmp(cliente1.cpf, cpf2) == 0) {
                  char senha[50];
                  printf("Digite sua senha: ");
                  scanf(" %s", senha);

                  while (strcmp(cliente1.senha, senha) != 0) {
                      printf("Senha incorreta. Digite novamente sua senha: ");
                      scanf(" %s", senha);
                  }

                  double valor_credito;
                  printf("Qual valor você deseja creditar: R$");
                  scanf("%lf", &valor_credito);

                  double valor = valor_credito;
                  double taxa = 0.0;

                  if (cliente1.tipo_conta == 'C') {
                      double novo_saldo = cliente1.saldo + (valor + taxa);

                      while (novo_saldo < -1000.0) {
                          printf("Limite de R$1000.00 atingido.\n");
                          printf("Digite um valor valido: R$");
                          scanf("%lf", &valor_credito);
                          valor = valor_credito;
                          novo_saldo = cliente1.saldo - (valor + taxa);
                      }

                      cliente1.saldo = novo_saldo;
                  } else {
                      double novo_saldo = cliente1.saldo + (valor + taxa);

                      while (novo_saldo < -5000.0) {
                          printf("Limite de R$5000.00 atingido.\n");
                          printf("Digite um valor valido: R$");
                          scanf("%lf", &valor_credito);
                          valor = valor_credito;
                          novo_saldo = cliente1.saldo + (valor + taxa);
                      }

                      cliente1.saldo = novo_saldo;
                  }

                  // Atualiza as transações do cliente
                  time_t current_time;
                  time(&current_time);
                  struct tm* local_time = localtime(&current_time);

                  sprintf(cliente1.transacoes[cliente.num_transacoes], "Data: %04d-%02d-%02d %02d:%02d:%02d / +R$%.2lf / Tarifa: R$%.2lf / Saldo: R$%.2lf",
                          local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
                          local_time->tm_hour, local_time->tm_min, local_time->tm_sec, valor, taxa, cliente1.saldo);

                  // Atualiza o número de transações
                  cliente1.num_transacoes++;

                  // Volta para a posição correta no arquivo
                  fseek(file1, -sizeof(Cliente), SEEK_CUR);

                  // Escreve o registro atualizado de volta no arquivo binário
                  fwrite(&cliente1, sizeof(Cliente), 1, file1);

                  printf("Saldo de R$%.2lf\n", cliente1.saldo);
                  fclose(file1);
                  return;
              }
          }

          printf("Cliente nao encontrado!\n");
          fclose(file1);
          return;
      }

// Operacao 8: Débito Automático
void debito_automatico() {
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

            // Obtêm a data e hora atual
            time_t current_time;
            time(&current_time);
            struct tm* local_time = localtime(&current_time);

            // Agenda o débito automático
            sprintf(cliente.transacoes[cliente.num_transacoes], "Debito automatico agendado para o dia %d/%02d/%04d / -R$%.2lf / Tarifa: R$0.00 / Saldo: R$%.2lf",
                    dia, local_time->tm_mon + 1, local_time->tm_year + 1900, valor, novo_saldo);

            cliente.num_transacoes++;
            cliente.saldo = novo_saldo;

            // Volta para a posição correta no arquivo
            fseek(file, -sizeof(Cliente), SEEK_CUR);

            // Escreve o registro atualizado de volta no arquivo binário
            fwrite(&cliente, sizeof(Cliente), 1, file);

            printf("Debito automatico agendado com sucesso!\n");
            fclose(file);
            return;
        }
    }

    printf("Cliente nao encontrado!\n");
    fclose(file);
}
