#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define BUFFER_SIZE 1024

int main() {
  int op;

  HANDLE hPipe;
  char buffer[BUFFER_SIZE];
  DWORD dwRead, dwWritten;



  // Send data to server
  // ------------------------------------------------------------------------------------------------------------------

  while (1) {
    printf("\nDigite uma operacao:\n");
    printf("1: INSERT\n");
    printf("2: DELETE\n");
    printf("0: SAIR\n\n");
    scanf("%d", &op);
    getchar();

    if (op == 1) {
      int id;
      char nome[50];
      printf("INSERT id=");
      scanf("%d", &id);
      getchar();
      printf("INSERT id=%d nome=", id);
      fgets(nome, sizeof(nome), stdin);
      nome[strcspn(nome, "\n")] = 0;
      snprintf(buffer, BUFFER_SIZE, "INSERT %d %s", id, nome);

    } else if (op == 2) {
        int id;
        printf("DELETE id=");
        scanf("%d", &id);
        getchar();
        snprintf(buffer, BUFFER_SIZE, "DELETE %d", id);

    } else if (op == 0) {
      break;

    } else {
      printf("Operacao invalida.\n");

    }

      // Open named pipe
    hPipe = CreateFile("\\\\.\\pipe\\pipeso", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        printf("Falha em abrir o pipe. Codigo do erro: %lu\n", GetLastError());
        return 1;
    }

    printf("Conectado ao servidor!\n");

    if (!WriteFile(hPipe, buffer, strlen(buffer) + 1, &dwWritten, NULL))
    {
        printf("Falha em escrever no pipe. Codigo do erro: %lu\n", GetLastError());
        CloseHandle(hPipe);
        return 1;
    }

    printf("Dado enviado ao servidor.\n");

    // Read data from server
    if (!ReadFile(hPipe, buffer, BUFFER_SIZE, &dwRead, NULL))
    {
        printf("Falha em ler do pipe. Codigo do erro: %lu\n", GetLastError());
        CloseHandle(hPipe);
        return 1;
    }

    printf("Dado recebido: %s\n", buffer);

    // Close named pipe and exit
    CloseHandle(hPipe);
  }
  return 0;
}