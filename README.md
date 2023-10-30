# projeto2DA

*Banco QuemPoupaTem*

Este projeto visa a criação de um sistema bancário em linguagem C para o novo banco QuemPoupaTem. O sistema é responsável por gerenciar contas bancárias, permitindo a criação de novos clientes, remoção de clientes, listagem de clientes, realização de débitos, depósitos, geração de extratos e transferências entre contas. O sistema é baseado em um arquivo binário que armazena as informações dos clientes de forma permanente.

O código apresenta alguns bugs por conta do buffer na IDE utilizada (Replit), entretanto, isso não afeta a usabilidade do código de nenhuma maneira.

-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

Descrição das principais funções utilizadas para a operação do banco (contidas no arquivo functions.c):

*novo_cliente*: Essa função permite a criação de um novo cliente no banco. Ela coleta informações do cliente, como CPF, tipo de conta, valor inicial e senha, e as armazena em um arquivo binário.

*apagar_cpf*: Esta função permite a exclusão de um cliente com base no CPF e na senha fornecidos. O cliente é removido do arquivo binário de clientes.

*listar*: Essa função lista todos os clientes registrados no banco. Ela lê o arquivo binário de clientes e exibe informações como CPF, tipo de conta, saldo e senha de cada cliente.

*debito*: Permite a realização de um débito na conta de um cliente. O usuário deve fornecer o CPF, senha e o valor do débito, com taxas calculadas com base no tipo de conta.

*deposito*: Essa função realiza um depósito na conta de um cliente. O usuário deve fornecer o CPF, senha e o valor a ser depositado, com taxas calculadas com base no tipo de conta.

*extrato*: Gera um extrato bancário para um cliente, exibindo todas as transações realizadas por ele. O extrato é salvo em um arquivo de texto e inclui informações como data, valores, tarifas e saldo.

*realizarTransferencia*: Permite que um cliente realize uma transferência de fundos para outra conta. O cliente fornece o CPF da conta de origem, a conta de destino e o valor. A função cuida de verificar a senha, o saldo e as taxas de transferência.

*debito_automatico*: Agendamento de débito automático para um cliente. O cliente fornece o valor e o dia do mês em que o débito será efetuado. A função verifica a senha, o limite da conta e agenda a transação.
