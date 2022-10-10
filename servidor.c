#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	char rtn[50];		// String de retorno do get	
	typedef struct {
		int cod;
		int pos;
		char msg[50];
	} Message;		// Estrutura da mensagem
	Message msg;		// Mensagem recebida
	char file[50][50];	// Arquivo a ser editado

	// Variáveis do socket
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;

	// Configurando o socket
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);
	while(1) {		
		printf("server waiting\n");
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);
		read(client_sockfd, &msg, 1);
		if (msg.cod == 0)
			strcpy(file[msg.pos], msg.msg);
		strcpy(rtn, file[msg.pos]);
		write(client_sockfd, &rtn, 1);
		close(client_sockfd);
	}
}
