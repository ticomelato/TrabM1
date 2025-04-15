#ifndef BANCO_H
#define BANCO_H

#include <pthread.h>

#define THREAD_NUM 4

#define BANCO_TXT "banco.txt"

typedef struct {
    int id;
    char nome[50];
} Registro;

extern pthread_mutex_t mutex;

void* iniciar_thread(void* args);
void* processar_requisicao(void* arg);

#endif
