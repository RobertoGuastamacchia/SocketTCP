#include "..\..\Functions\functions.h"
#define SERVER_IP "127.0.0.1"

int main() {
	int port = PROTOPORT;
		if (port < 0) {
			printf("Port %s is not valid.\n");
			return 0;
		}else{
			WSADATA data;
			int check;
			check=startUP(&data);
			if(check==1){
				int s_socket =-1;
				if( s_socket = createSocket(&s_socket)){ // create the sochet
					// Assignment address to the socket
					struct sockaddr_in socketAddr;
					configSocket(&socketAddr,SERVER_IP,port);
					int ck = 1;
					if (bind(s_socket, (struct sockaddr*) &socketAddr, sizeof(socketAddr)) < 0) { // bind() the socket
						printf("Error - bind() failed.\n");
						closesocket(s_socket);
						clearwinsock();
						return -1;
					}else{
						printf("Success - bind().\n");
						if (listen (s_socket, QLEN) < 0) { // server listening
							printf("Error - listen() failed.\n");
							clearwinsock(s_socket);
							WSACleanup();
							return -1;
						}
						else{
							printf("Success - listen().\n");
							struct sockaddr_in c_socket; // struttura per l'indirizzo del client
							int client_socket; // descritore del socket client
							int client_len; // lunghezza dell'indirizzo del client
							printf("Waiting for a client to connect...\n");
							client_len = sizeof(c_socket); // inizializza la lunghezza del socket client
							while ((client_socket = accept(s_socket, (struct sockaddr*)&c_socket, &client_len)) >=0) {
								printf("Connection established with  %s:%d\n", inet_ntoa(c_socket.sin_addr),htons(c_socket.sin_port));
								int err = 0;
								char buf[BUFFERSIZE]; // buffer for the input message
								memset(buf, 0, BUFFERSIZE);
								printf("Waiting client message...\n");
								char outputString[MaxStringOperation];
								while (err==0) {
									if (reciveMessage(client_socket, buf) == 1) // recive message
									{
										if(strcmp(buf,"=\n")!=0 && strcmp(buf,"=")!=0){
											elaborateCalc(buf,outputString); // elaborate the input string
											if (sendMessage(client_socket, outputString)==-1){ // send message to client
												err=1;
												continue;
											}else{
												memset(buf, 0, BUFFERSIZE);
											}
										}
										else{
											printf("Client connection closing...\n");
											closesocket(client_socket);
											err=1;
										}
									}
									else{
										err = 1;
									}
								}
								if (client_socket==-1) {
									printf("Error - association client-server failed.\n");
									closesocket(client_socket);
									clearwinsock();
								}
								err = 0;
							}
						}
					}
				}
			}
		}
		system("PAUSE");
}


int add (int op1,int op2){
	int res = 0;
	res = op1+op2;
	return res;
}
int mult (int op1,int op2){
	int res = 0;
	res = op1*op2;
	return res;
}
int sub (int op1,int op2){
	int res = 0;
	res = op1-op2;
	return res;
}
int division (int op1,int op2){
	int res = 0;
	res = op1/op2;
	return res;
}
void elaborateCalc(char* buf,char* outputString){
	float output;
	char op[1];
	int op1;
	int op2;
	char *ptr = strtok(buf, "_");
	int i = 0;
	while(ptr != NULL){
		if(i==0){
			strcpy(op,ptr);
		}
		if(i==1){
			op1=atoi(ptr);
		}
		if(i==2){
			op2=atoi(ptr);
		}
		ptr = strtok(NULL, "_");
		i++;
	}
	if(strcmp(op,"+")==0){
		output=add(op1,op2);
		itoa(output,outputString,10);
	}
	else if(strcmp(op,"-")==0){
		output=sub(op1,op2);
		itoa(output,outputString,10);
	}
	else if(strcmp(op,"*")==0){
		output=mult(op1,op2);
		itoa(output,outputString,10);
	}
	else if(strcmp(op,"/")==0){
		if(op2 != 0){
			output=division(op1,op2);
			itoa(output,outputString,10);
		}else{
			strcpy(outputString,"Error, No valid operation , op2 < 0.");
		}
	}
	else{
		strcpy(outputString,"Error, No valid operator");
	}
}

