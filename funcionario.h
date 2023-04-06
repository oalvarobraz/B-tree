#ifndef TRABALHOAEDS_FUNCIONARIO_H
#define TRABALHOAEDS_FUNCIONARIO_H

/*
 Dentro deste arquivo estão as declarações das funções, além disso eu importei algumas bibliotecas
 para o funcionamento do meu código como a lista.h, para que eu pudesse utilizar a strcut Lista
 e também a time.h, que foi utilizada para calcular o tempo de execução do algoritmo, eu podia ter
 calculado o tempo de execução dentro ou fora do algoritmo, entretanto eu preferi calcular dentro do
 algoritmo mesmo.
 Além do que foi falado acima, eu criei uma struct Funcionario
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>
#include "lista.h"

typedef struct Funcionario {
    int cod;
    char nome[50];
    double salario;
} TFunc;

// Imprime funcionario
void imprime(TFunc *func);

// Cria funcionario. Lembrar de usar free(funcionario)
TFunc *funcionario(int cod, char *nome, double salario);

// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out);

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le(FILE *in);

// Retorna tamanho do funcionario em bytes
int tamanho();

// Imprime arquivo
void imprime_arquivo(FILE *arq);

// Retorna o tamanho do arquivo
int tamanho_arquivo(FILE *arq);



#endif //TRABALHOAEDS_FUNCIONARIO_H
