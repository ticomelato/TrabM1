# Avaliação M1 - IPC, Threads e Paralelismo
Alunos: Pedro J. Garcia, Thiago Z. Melato

Projeto desenvolvido como parte da disciplina de Sistemas Operacionais da Escola Politécnica da UNIVALI, ministrada pelo professor Felipe Viel.

## Dependências
Para compilar e executar o projeto, são necessárias as seguintes ferramentas:
- **GCC**: Compilador para C/C++.
- **GDB**: Depurador para C/C++.
- **Pthreads**: Biblioteca para threads (já incluída no GCC em sistemas Linux e disponível no Windows via MinGW ou MSYS2).
- **API do Windows**: Comunicação via *pipe* nomeado.

## Uso
### Configuração do ambiente
1. Certifique-se de que o GCC e o GDB estão instalados e configurados no PATH do sistema.
2. No Windows, use o [MSYS2](https://www.msys2.org/) para instalar as dependências:
   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-pthreads
   ```

### Compilação
1. Abra o terminal no diretório do projeto.
2. Compile o servidor:
   ```bash
   gcc -o servidor servidor.c -lpthread
   ```
3. Compile o cliente:
   ```bash
   gcc -o cliente cliente.c
   ```

### Execução
1. Inicie o servidor:
   ```bash
   ./servidor
   ```
2. Em outro terminal, execute o cliente:
   ```bash
   ./cliente
   ```

## Operações suportadas
- **INSERT**: Adiciona um novo registro ao banco de dados.
- **SELECT**: Busca um registro pelo ID.
- **UPDATE**: Atualiza o nome de um registro existente.
- **DELETE**: Remove um registro pelo ID.

## Créditos
Partes desta implementação foram baseadas em códigos disponibilizados pelo professor na plataforma Notion. A implementação de *thread pool* é baseada na disponível no repositório [VielF/SO-Codes](https://github.com/VielF/SO-Codes/blob/main/Threads/thread_pool_C_ex.c).