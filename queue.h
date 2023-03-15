//
// Created by Alvaro on 15/03/2023.
//

#ifndef ARVORE_B_QUEUE_H
#define ARVORE_B_QUEUE_H


#include "arvoreB.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct TFunc TFunc;
typedef struct btNode btNode;
typedef struct bTree bTree;

typedef struct queue{
    int size;
    int front;
    int rear;
    int itemCount;
    btNode *list;
}queue;

queue *createQueue(int size);
btNode peek(queue *q);
bool isEmpty(queue *q);
bool isFull(queue *q);
int size(queue *q);
void insert(queue *q ,btNode data);
btNode removeData(queue *q);

#endif //ARVORE_B_QUEUE_H
