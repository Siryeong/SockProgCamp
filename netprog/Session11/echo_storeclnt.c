#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>

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

	pid = fork();
	if (pid == 0)
		write_routine(sock, buf);
	else 
		read_routine(sock, buf);

	close(sock);
	return 0;
}

void read_routine(int sock, char *buf)
{
	int fds[2];
	int pid2; 	
	FILE *fp; 

	fp = fopen("echoclntmsg.txt", "w");
	
	// TODO: create pipe & fork 
	pipe(fds);
	pid2 = fork();

	// TODO: store received echo message
	if(pid2 == 0){
		char msgbuf[BUF_SIZE];
		int i, len;

		while(1){
			len = read(fds[0], msgbuf, BUF_SIZE);
			if(len == 0) break;
			fwrite((void*)msgbuf, 1, len, fp);
			// flush??
		}
		fclose(fp);
		return;
	}

	while (1)
	{
		int str_len = read(sock, buf, BUF_SIZE);
		write(fds[1], buf, str_len);
		if (str_len == 0)
		{
			//fclose(fp);
			//kill(pid2, SIGKILL);
			close(fds[1]);
			return;
		}

		buf[str_len] = 0;
		printf("Message from server: %s", buf);
	}
}
void write_routine(int sock, char *buf)
{
	while (1)
	{
		fgets (buf, BUF_SIZE, stdin);

		if (!strcmp(buf,"q\n") || !strcmp(buf,"Q\n"))
		{	
			shutdown(sock, SHUT_WR);
			return;
		}
		write(sock, buf, strlen(buf));
	}
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}