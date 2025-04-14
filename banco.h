#ifndef BANCO_H
#define BANCO_H

#include <pthread.h>

#define BANCO_TXT "banco.txt"

typedef struct {
    int id;
    char nome[50];
} Registro;

extern pthread_mutex_t mutex;

void* processar_requisicao(void* arg);

#endif
