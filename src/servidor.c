#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_LINES 50		// Número máximo de linhas
#define LINE_SIZE 50		// Tamanho máximo da linha
#define MAX_CLIENTS 2		// Número máximo de clientes simultâneos

// Estrutura da mensagem
typedef struct {
	int cod;
	int index;
	char line[LINE_SIZE];
} Message;
// Arquivo de texto para edição
char file[NUM_LINES][LINE_SIZE];

// Variáveis do socket
int server_sockfd, client_sockfd;
int server_len, client_len;
struct sockaddr_un server_address;
struct sockaddr_un client_address;

// Retorna o conteúdo do arquivo na posição index.
const char* get_line(int index) {
	return file[index];
}

// Adiciona o conteúdo da variável line no arquivo, na posição index.
void add_line(int index, const char* line) {
	strcpy(file[index], line);
}

int main() {
	char rtn[LINE_SIZE];		// String de retorno	
	Message msg;						// Mensagem recebida

	// Configurando o socket
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, MAX_CLIENTS);

	while(1) {		
		printf("Server waiting...\n");
		
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);
		read(client_sockfd, &msg, sizeof(msg));
		
		if (msg.cod == 0)
			add_line(msg.index, msg.line);

		strcpy(rtn, get_line(msg.index));
		write(client_sockfd, &rtn, sizeof(rtn));
		close(client_sockfd);
	}
}
