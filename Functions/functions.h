#if defined WIN32
#include <winsock.h>
#else
#define closesocket close  //in unix system closesocket is not defined but is used close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define QLEN 5 // request queue
#define PROTOPORT 57015  //  default port
#define BUFFERSIZE 512 // buffer size
#define PORT 48000
#define MaxStringOperation 20
#define DEBUG 1

void clearwinsock() {
#if defined WIN32
WSACleanup();
#endif
}


int startUP(WSADATA *data){
	int check_stp = WSAStartup(MAKEWORD(2,2),data);
	if(check_stp != NO_ERROR){
		printf("Error - WSAStartup().\n");
		return -1;
	}else{
		printf("Success - WSAStartup().\n");
		return 1;
	}
}

int createSocket(){
	int socketTarget = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketTarget < 0) {
		printf("Error - socket creation failed.\n");
		closesocket(socketTarget);
		clearwinsock();
		return -1;
	}else{
		printf("Success - socket creation.\n");
		return socketTarget;
	}
}

void configSocket(struct sockaddr_in *socketAddr,char* ip,int port){
	memset(socketAddr, 0, sizeof(struct sockaddr_in));
	socketAddr->sin_family = AF_INET;
	socketAddr->sin_addr.s_addr = inet_addr(ip); // localhost
	socketAddr->sin_port = htons(port); // service port
}

int sendMessage(int socketTarget, char messaggio[]){
	int string_len = strlen(messaggio);
	if (send(socketTarget, messaggio, string_len, 0) != string_len){
		printf("Error - send() sent a different number of bytes than expected.\n");
		return -1;
	}else{
		printf("Message send to the server.\n");
	}
	return 1;
}

int reciveMessage(int socketTarget,char messaggio[]){
	if ((recv(socketTarget, messaggio, BUFFERSIZE - 1,0)) <= 0)
	{
		printf("Error - recv() failed or connection closed prematurely...\n");
		closesocket(socketTarget);
		//clearwinsock();
		return -1;
	}
	return 1;
}
