#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char *argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_STREAM, 0);  
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));
	
	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	
	// TODO: fork
	pid = fork();

	// TODO: parent process calls read routine and 
	//       child process calls write routine
	
	if(pid == 0) // parent
		read_routine(sock, buf);
	else // child
		write_routine(sock, buf);

	
	close(sock);
	return 0;
}

void read_routine(int sock, char *buf)
{
	int read_size;
	while (1)
	{
		// TODO: Read message from the echo server
		read_size = read(sock, buf, BUF_SIZE);
		if(read_size == 0) break;
		printf("echo : %s\n", buf);
	}
}
void write_routine(int sock, char *buf)
{
	while (1)
	{
		fputs("Input message(Q to quit): ", stdout);
		// TODO: Write message to the echo server
		fgets(buf, BUF_SIZE, stdin);
		if (!strcmp(buf,"q\n") || !strcmp(buf,"Q\n"))
			break;
		write(sock, buf, strlen(buf));
	}
	shutdown(sock, SHUT_WR);
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}