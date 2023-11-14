// Vinícius de Castro Duarte - 24.123.073-9
// Renan Ivaskevicius Vieira - 24.123.069-7

#ifndef UNTITLED_FUNCTIONS_H
#define UNTITLED_FUNCTIONS_H

typedef struct {
    char cpf[15];
    char tipo_conta;
    double saldo;
    char senha[50];
    char transacoes[1000][100];
    int num_transacoes;
} Cliente;

char confirmacao(int operacao);

int confirma_abertura_arquivo(char arquivo[30]);

int arquivo_escrito(char arquivo[30]);

void novo_cliente(Cliente* clientes, int* numClientes);

void apagar_cpf(Cliente* clientes, int* numClientes);

void listar(void);

void debito(Cliente* clientes, int* numClientes);

void deposito(Cliente* clientes, int* numClientes);

#endif
