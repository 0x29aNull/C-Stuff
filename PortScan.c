/* Threaded Port Scanner in C - M Logsdon - 10/28/22 */

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <errno.h> 
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

int resolver(char* hostname , char* ip)
{
	struct hostent *host;
	struct in_addr **rhost;
	if ((host = gethostbyname(hostname)) == NULL)
	{
		herror("gethostbyname error");
		return 1;
	}
	rhost = (struct in_addr **) host->h_addr_list;
	for(int i = 0; rhost[i] != NULL; i++)
	{
		strcpy(ip , inet_ntoa(*rhost[i]));
		return 0;
	}
	return 1;
}

int connector(char* ip, char* portnum)
{
	int x = (size_t)portnum;
	int sock = 0, client_fd;
	struct sockaddr_in serv_addr;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\nFailed to create socket");
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);

	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid IP");
	}

	if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr,
		sizeof(serv_addr))) < 0) {
		// printf("\nConnection Failed");
		return -1;
	}
	printf("\nConnection successful on %d", x);
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("\nUsage: ./fag <hostname/ip>");
		exit(1);
	}

	char *hostname = argv[1];
	char ip[100];
	resolver(hostname,ip);
	printf("%s resolves to %s", hostname , ip);
	for (int portnum = 0; portnum < 500; portnum++) {
		connector(ip, portnum);
	}
	return 0;
}
