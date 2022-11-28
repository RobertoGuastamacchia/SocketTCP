#include "..\..\Functions\functions.h"
int TCPClient();

int main(void) {
	int i = TCPClient();
	system("pause");

}

int TCPClient(){
	WSADATA data;
	int check;
	check=startUP(&data);
	if(check==1){
		int c_socket = -1;
		if(c_socket = createSocket(&c_socket)){ // create the socket
			// Assignment address to the socket
			char ip[15];
			int port;
			printf("Write 'server' for default configurations, else write any keys.\n");
			//char inputSel[MaxStringOperation]="";
			//fgets(inputSel, sizeof(inputSel), stdin); // input a string

			if(DEBUG==0){
				printf("Inserisci l'ip del server\n");
				scanf("%s",&ip);
				printf("Inserisci il numero della porta\n");
				scanf("%d",&port);
			}else{
				strcpy(ip,"127.0.0.1");
				port=57015;
			}

			struct sockaddr_in socketAddr;
			configSocket(&socketAddr,ip,port);
			// server connection
			if (connect(c_socket, (struct sockaddr *)&socketAddr, sizeof(socketAddr))< 0)
			{
				printf( "Error, during the server connection.\n" );
				closesocket(c_socket);
				clearwinsock();
				return -1;
			}
			else{
				printf("Connection with the server established.\n");
				char input[MaxStringOperation];
				char input_string[MaxStringOperation];
				char div[2] = " ";
				printMenu();
				fflush(stdin);
				strcpy(input, "");
				fgets(input, sizeof(input), stdin); // input a string

				while(strcmp(input,"=\n")!=0 && strcmp(input,"=")!=0){
					char *ptr = strtok(input, div);
					int i = 0;
					while(ptr != NULL){ // parsing the string
						if(i==0){
							strcpy(input_string,ptr);
						}
						else{
							strcat(input_string,"_");
							strcat(input_string,ptr);
						}
						ptr = strtok(NULL, div);
						i++;
					}
					int string_len = strlen(input_string);
					if (sendMessage(c_socket, input_string)==-1){ // send message to server
						return -1;
					}
					else{
						char buf[BUFFERSIZE]; // message buffer
						memset(buf, 0, BUFFERSIZE); // set buffer at 0
						printf("Waiting for a response from the server...\n");
						if (reciveMessage(c_socket, buf) == -1) //recive response from the server
						{
							return -1;
						}
						else{
							printf("Result: %s\n\n\n", buf);
						}
					}
					printMenu();
					fflush(stdin);
					input[MaxStringOperation] = ""; // reset the string
					fgets(input, sizeof(input), stdin);// input a string
				}
				if (sendMessage(c_socket, input)==-1){ // send message to server
					return -1;
				}
				else{
					printf("Connection closed...\n");
					closesocket(c_socket);
					clearwinsock();
					return 0;
				}
			}
		}
	}
}


void printMenu(){
	printf("-------------- MENU ------------\n");
	printf("1) Add [+ Op1 Op2]\n");
	printf("2) Subtraction [- Op1 Op2]\n");
	printf("3) Division [/ Op1 Op2]\n");
	printf("4) Multiplication [* Op1 Op2]\n");
	printf("5) Exit [=]\n");
	printf("--------------------------------\n");
	printf("Insert operation and operands: \n");
}
