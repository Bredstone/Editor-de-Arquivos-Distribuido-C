#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  int opcao, line_num = 0;

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
    scanf("%d", &line_num);
    if(line_num < 0 || line_num > 300) printf("Linha inválida!");

      break;

    case 2:

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