/*
 * Server.c
 *
 *  Created on: 14/ott/2019
 *      Author: Danilo
 */
#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define QLEN 6
#define PROTOPORT 27015;


void ErrorHandler(char *errorMessage){
	printf("%s", errorMessage);
}

void ClearWinSock(){
#if defined WIN32
	WSACleanup();
#endif
}


int main(int argc,char *argv[]){
int port;
if (argc > 1){
	port = atoi(argv[1]);
}
else {
	port = PROTOPORT;
}
// use default port number
	if (port < 0) {
		printf("bad port number %s \n", argv[1]);
		return 0;
	}

//inizializzazione WinSock
#if defined WIN32
	WSADATA wsaData;
	int iResult=WSAStartup(MAKEWORD(2,2), &wsaData);//restituisce 0 se va bene

	if(iResult!=0){
		ErrorHandler("Error at WSAStartup.\n");
		return 0;
	}
#endif
//creazione della socket
	int MySocket;

	MySocket= socket(PF_INET,SOCK_STREAM,IPPROTO_TCP); //return <0 if fail

	if(MySocket<0){
		ErrorHandler("Error at creating Socket.\n");
		system("pause");
		ClearWinSock();
		return -1;
	}

//assegnazione di un indirizzo alla socket

	struct sockaddr_in sad;  //(Server Address)
	memset(&sad,0,sizeof(sad));
	sad.sin_family=AF_INET;
	sad.sin_addr.s_addr=inet_addr("127.0.0.1");
	sad.sin_port=htons(port);
	//bind()
	if(bind(MySocket,(struct sockaddr*)&sad, sizeof(sad))<0){
		ErrorHandler("bind() failed.\n");
		system("pause");
		closesocket(MySocket);
		ClearWinSock();
		return -1;
	}

//settaggio del socket all'ascolto
	if(listen(MySocket,QLEN)<0){
		ErrorHandler("listen() failed.\n");
		system("pause");
		closesocket(MySocket);
		ClearWinSock();
		return -1;
	}
//accettiamo una nuova connesione

	struct sockaddr_in cad; //(client address)
	int ClientSocket;
	int ClientLen; //lunghezza dell'indirizzo del client

	printf("Waiting for a client to connect...");
	int bytesrcvd=1;
	char buf[512];

	while(1){
		ClientLen=sizeof(cad);
		ClientSocket=accept(MySocket, (struct sockaddr*) &cad, &ClientLen);
		if(ClientSocket<0){
			ErrorHandler("accept() Failed.\n");
			system("pause");
			closesocket(MySocket);
			ClearWinSock();
			return 0;
			}
		printf("\nHandling client: %s.\n",inet_ntoa(cad.sin_addr));


		char str[]="connessione avvenuta";

		int strl=strlen(str);

	//primo punto (invio al client il mex di connessione)

		if(send(ClientSocket,str, strl,0)!= strl){
					ErrorHandler("Fail to send().\n ");
					system("pause");
					closesocket(MySocket);
					ClearWinSock();
					return -1;


			}
	//2 punto

		if((bytesrcvd=recv(ClientSocket,buf,511,0))<=0){
							ErrorHandler("Receive failed.\n");
							system("pause");
							closesocket(MySocket);
							ClearWinSock();
					 		return -1;
						}
		buf[bytesrcvd]='\0';//appendo \0 alla fine della stringa per printarla
		printf("%s",buf);     //print the echo buffer

		char maius[]="";
		strcpy(maius,strupr(buf));


		if(send(ClientSocket,maius,bytesrcvd,0)!=bytesrcvd){
			ErrorHandler("Fail to send().\n ");
								system("pause");
								closesocket(MySocket);
								ClearWinSock();
								return -1;
		}
if((bytesrcvd=recv(ClientSocket,buf,511,0))<=0){
							ErrorHandler("Receive failed.\n");
							system("pause");
							closesocket(MySocket);
							ClearWinSock();
					 		return -1;
								}
buf[bytesrcvd]='\0';
printf("\n%s",buf);
strcpy(maius,strlwr(buf));
if(send(ClientSocket,maius,bytesrcvd,0)!=bytesrcvd){
			ErrorHandler("Fail to send().\n ");
								system("pause");
								closesocket(MySocket);
								ClearWinSock();
								return -1;
		}
//}
	}
	system("pause");
	closesocket(ClientSocket);
	return 0;
}


/*char b[]="";

		buf[bytesrcvd]='\0';//appendo \0 alla fine della stringa per printarla
		printf("%s",buf);  */   //print the echo buffer
