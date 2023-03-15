#ifndef ARVORE_B_ARVOREB_H
#define ARVORE_B_ARVOREB_H


#include <stdio.h>
#include <malloc.h>
#include "queue.h"


typedef struct funcionario {
    int codigo;
    float salario;
} func;

typedef struct btNode {
    int num_funcs;
    int flag_folha;
    int pos_in_disk;
    func *funcs;
    int *filhos;
} btNode;

typedef struct bTree {
    int order;
    btNode root;
    int node_count;
} bTree;

typedef struct queue queue;


int calcula_espacamento(int disk, int order);


btNode pesq_no(btNode node, int order, func key, FILE *fp);


void imprimir_func(bTree tree, func key, FILE *fp);


bTree *cria_arvore(int order);


int pesquisar(btNode node, int order, func key, FILE *fp);


void inserir(bTree *tree, func key, FILE *fp);


int remover(bTree *tree, func key, FILE *fp);


void imprimir_arvoreB(bTree *tree, queue *q, FILE *fp);


void libera_arvore(bTree *tree, FILE *fp);


func chave_max(btNode node, int order, FILE *fp);


func chave_min(btNode node, int order, FILE *fp);


btNode le_arq(int disk, int order, FILE *fp);


#endif //ARVORE_B_ARVOREB_H
