#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  int opcao, linha = 0;
  char texto[50], rtn[50];

  // Estrutura da mensagem
  typedef struct
  {
    int cod;
    int index;
    char line[50];
  } Message;

  Message msg;

  // Variáveis do socket
  int result;
  int sockfd;
  int len;
  struct sockaddr_un address;

  // Configurando o socket
  sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  address.sun_family = AF_UNIX;
  strcpy(address.sun_path, "server_socket");
  len = sizeof(address);
  result = connect(sockfd, (struct sockaddr *)&address, len);
  if (result == -1)
  {
    perror("oops: client1");
    exit(1);
  }

  while (1)
  {
    printf("\e[1;1H\e[2J");

    printf("Menu: \n");
    printf("1 - Adicionar linha\n");
    printf("2 - Ler linha\n");
    printf("3 - Sair\n");
    printf("Opção:");
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
      printf("Digite o número da linha:");
      scanf("%d", &linha);
      if (linha < 0 || linha > 50)
        printf("Linha inválida!");
      else
      {
        printf("Digite o texto:");
        scanf("%s", texto);

        msg.cod = opcao;
        msg.index = linha;
        strcpy(msg.line, texto);

        // Enviando mensagem
        write(sockfd, &msg, 1);
      }

      break;

    case 2:
      printf("Digite o número da linha:");
      scanf("%d", &linha);
      if (linha < 0 || linha > 50)
        printf("Linha inválida!");
      else
      {
        msg.cod = opcao;
        msg.index = linha;

        // Enviando mensagem
        write(sockfd, &msg, 1);

        // Resposta
        read(sockfd, &rtn, 1);
        if (strlen(rtn) == 0)
          printf("Linha vazia!\n");
        else
        {
          printf("%s\n", rtn);
        }
      }

      break;

    case 3:
      exit(0);
      break;

    default:
      printf("Opção inválida!\n");

      break;
    }

    sleep(2);
  }
}