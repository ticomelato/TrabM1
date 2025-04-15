#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "banco.h"

#define BUFFER_SIZE 1024

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* processar_requisicao(void* arg) {
    HANDLE hPipe = *(HANDLE*)arg;
    free(arg); // liberar a memória alocada para o HANDLE

    char buffer[BUFFER_SIZE];
    DWORD dwRead, dwWritten;

    // Ler do pipe
    if (!ReadFile(hPipe, buffer, BUFFER_SIZE, &dwRead, NULL)) {
        printf("Erro ao ler do pipe. Codigo: %d\n", GetLastError());
        CloseHandle(hPipe);
        return NULL;
    }

    char comando[10], nome[50];
    int id;

    if (sscanf(buffer, "INSERT %d %[^\n]", &id, nome) == 2) {
        pthread_mutex_lock(&mutex);
        FILE* banco = fopen("banco.txt", "a+");
        if (banco) {
            rewind(banco); // volta para o início

            char linha[100];
            int idExiste = 0;
            Registro r;

            // Verifica se o ID do registro a ser adicionado já existe no banco
            while (fgets(linha, sizeof(linha), banco)) {
                sscanf(linha, "%d|%[^\n]", &r.id, r.nome);
                if (r.id == id) {
                    idExiste = 1;
                    printf("Registro com ID %d ja existe.\n", id);
                    strcpy(buffer, "ID ja existente.");
                }
            }

            if (!idExiste) {
                fprintf(banco, "%d|%s\n", id, nome);
                printf("Registro adicionado: %d | %s\n", id, nome);
                strcpy(buffer, "Registro inserido com sucesso.");
            }

            fclose(banco);
        } else {
            printf("Erro ao abrir banco.txt\n");
            strcpy(buffer, "Erro ao acessar o banco.");
        }
        pthread_mutex_unlock(&mutex);
    }
    else if (sscanf(buffer, "DELETE %d", &id) == 1) {
        pthread_mutex_lock(&mutex);
        FILE* banco = fopen("banco.txt", "r");
        FILE* temp = fopen("temp.txt", "w");
        if (!banco || !temp) {
            printf("Erro ao abrir arquivos.\n");
            strcpy(buffer, "Erro ao acessar os arquivos.");
            pthread_mutex_unlock(&mutex);
            CloseHandle(hPipe);
            return NULL;
        }

        char linha[100];
        int encontrado = 0;
        Registro r;

        while (fgets(linha, sizeof(linha), banco)) {
            sscanf(linha, "%d|%[^\n]", &r.id, r.nome);
            if (r.id != id) {
                fprintf(temp, "%s", linha);
            } else {
                encontrado = 1;
            }
        }

        fclose(banco);
        fclose(temp);
        remove("banco.txt");
        rename("temp.txt", "banco.txt");

        if (encontrado) {
            printf("Registro com ID %d removido.\n", id);
            strcpy(buffer, "Registro removido com sucesso.");
        } else {
            printf("ID %d nao encontrado.\n", id);
            strcpy(buffer, "ID nao encontrado.");
        }

        pthread_mutex_unlock(&mutex);
    }
    else {
        printf("Comando invalido: %s\n", buffer);
        strcpy(buffer, "Comando invalido.");
    }

    // Enviar resposta ao cliente
    WriteFile(hPipe, buffer, strlen(buffer) + 1, &dwWritten, NULL);
    CloseHandle(hPipe);
    return NULL;
}

int main() {
    HANDLE hPipe;

    printf("Servidor iniciado. Aguardando conexoes...\n");

    while (1) {
        hPipe = CreateNamedPipe(
            "\\\\.\\pipe\\pipeso",
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            BUFFER_SIZE,
            BUFFER_SIZE,
            0,
            NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            printf("Erro ao criar o pipe. Codigo: %d\n", GetLastError());
            return 1;
        }

        if (!ConnectNamedPipe(hPipe, NULL)) {
            printf("Erro ao conectar ao cliente. Codigo: %d\n", GetLastError());
            CloseHandle(hPipe);
            continue;
        }

        printf("Cliente conectado!\n");

        // Aloca o handle e passa para a thread
        HANDLE* hPipeArg = malloc(sizeof(HANDLE));
        *hPipeArg = hPipe;

        pthread_t tid;
        pthread_create(&tid, NULL, processar_requisicao, hPipeArg);
        pthread_detach(tid); // thread destacada, não precisamos dar join
    }

    return 0;
}