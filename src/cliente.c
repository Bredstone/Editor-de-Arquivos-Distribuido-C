#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

// For threading, link with lpthread
#include <pthread.h>
#include <semaphore.h>

// Variáveis do socket
int result;
int sockfd;
int len;
struct sockaddr_un address;

void *clienthread(void *args)
{

  int client_request = *((int *)args);

  // Configurando o socket
  sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  address.sun_family = AF_UNIX;
  strcpy(address.sun_path, "server_socket");
  len = sizeof(address);
  result = connect(sockfd, (struct sockaddr *)&address, len);

  // Check for connection error
  if (result < 0)
  {
    puts("Error\n");
    return 0;
  }

  printf("Connection established\n");

  // Send data to the socket
  send(sockfd, &client_request, sizeof(client_request), 0);

  // Close the connection
  close(sockfd);
  pthread_exit(NULL);

  return 0;
}

int main()
{
  int opcao, linha = 0;
  char texto[50], rtn[50];
  int teste = 1;
  pthread_t tid;

  // Estrutura da mensagem
  typedef struct
  {
    int cod;
    int index;
    char line[50];
  } Message;

  Message msg;

  do
  {
    printf("\e[1;1H\e[2J");
    opcao = 0;

    printf("Menu: \n");
    printf("1 - Adicionar linha\n");
    printf("2 - Ler linha\n");
    printf("3 - Sair\n");
    printf("Opção:");
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
      printf("Digite o número da linha: ");
      scanf("%d", &linha);
      if (linha < 0 || linha > 50)
      {
        printf("Linha inválida!\n");
      }
      else
      {
        printf("Digite o texto: ");
        scanf("%s", texto);

        int client_request = 1;

        // Create thread
        pthread_create(&tid, NULL, clienthread, &client_request);

        msg.cod = 0;
        msg.index = linha;
        strcpy(msg.line, texto);

        // Enviando mensagem
        write(sockfd, &msg, 1);
      }
      sleep(1);

      break;

    case 2:
      printf("Digite o número da linha: ");
      scanf("%d", &linha);
      if (linha < 0 || linha > 50)
      {
        printf("Linha inválida!\n");
      }
      else
      {
        int client_request = 2;

        // Create thread
        pthread_create(&tid, NULL, clienthread, &client_request);

        msg.cod = 1;
        msg.index = linha;

        // Enviando mensagem
        write(sockfd, &msg, 1);

        // Resposta
        read(sockfd, &rtn, 1);

        if (strlen(rtn) == 0)
        {
          printf("Linha vazia!\n");
        }
        else
        {
          printf("%s\n", rtn);
        }
      }
      sleep(1);

      break;

    case 3:
      exit(0);
      break;

    default:
      printf("Opção inválida!\n");
      sleep(1);

      break;
    }

    // Suspend execution of calling thread
    pthread_join(tid, NULL);

  } while (opcao != 3);
}