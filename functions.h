// Vinícius de Castro Duarte - 24.123.073-9
// Renan Ivaskevicius Vieira - 24.123.069-7

#ifndef UNTITLED_FUNCTIONS_H
#define UNTITLED_FUNCTIONS_H

typedef struct {
    char razao_social[100];
    char cpf[15];
    char tipo_conta;
    double saldo;
    char senha[50];
    char transacoes[1000][100];
    int num_transacoes;
} Cliente;

char confirmacao(int operacao);

char* input_cnpj();

void novo_cliente();

void apaga_cliente();

void apagar_cnpj();

void listar();

void debito();

void deposito();

void extrato(const char* cnpj);

void transferencia_contas();

void debito_automatico();

#endif
