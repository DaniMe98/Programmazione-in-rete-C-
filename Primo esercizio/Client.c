/*
 * Client.c
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
//#include <string.h>
#include <stdlib.h>
#define PROTOPORT 27015	//numero porta default
#define BUFFERSIZE 512

void Errorhandler(char *s){
	printf("%s",s);
}

void ClearWinsock(){
#if defined WIN32
	WSACleanup();
#endif
}

char finale[]="";
char dfinale[]="";

void stampa(){
	printf("La prima in maiuscolo: %s",finale,"\nLa seconda in minuscolo: %s",dfinale);
}

int main(void){

	//inizializzo WinSock
#if defined WIN32
	WSADATA wsaData;
	int iResult=WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult!=0){
		Errorhandler("error at WSAStartup().\n");
		return -1;
	}
#endif

	//creazione della socket
	int CSocket;
	CSocket=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(CSocket<0){
		Errorhandler("Error in creating the socket.\n");
		closesocket(CSocket);
		ClearWinsock();
		return -1;
	}

	//costruzione dell'indirizzo server

	struct sockaddr_in sad;
	memset(&sad,0,sizeof(sad));
	sad.sin_family=AF_INET;
	sad.sin_addr.s_addr=inet_addr("127.0.0.1");
	sad.sin_port=htons(5193);


	//connessione al server
	if(connect(CSocket, (struct sockaddr*) &sad, sizeof(sad))<0){
		Errorhandler("Error in connect().\n");
		system("pause");
		closesocket(CSocket);
		ClearWinsock();
		return -1;
	}

	//ricevo la stringa di connessione avvenuta

	char buf[512]="c";
	int bytesrcvd;

	if((bytesrcvd=recv(CSocket,buf,BUFFERSIZE-1,0))<=0){
				Errorhandler("Receive failed.\n");
				system("pause");
				closesocket(CSocket);
				ClearWinsock();
				return -1;
			}

	buf[bytesrcvd]='\0';   //appendo \0 alla fine della stringa per printarla
	printf("%s",buf);     //print the echo buffer

	//2 punto (leggo due stringhe da tastiera e le invio al server)

	char prima[50];
	char seconda[50];

	printf("\nInserisci la prima stringa: ");
	scanf("%[^\n]%*c",prima);
	int l1=strlen(prima);

	//
	if(send(CSocket,prima, l1,0)!= l1){
				Errorhandler("Fail to send().\n ");
				system("pause");
				closesocket(CSocket);
				ClearWinsock();
				return -1;

			}

	if((bytesrcvd=recv(CSocket,prima,BUFFERSIZE-1,0))<=0){
		Errorhandler("Receive() failed.\n");
		system("pause");
		closesocket(CSocket);
		ClearWinsock();

	}
	prima[bytesrcvd]='\0';

	strcpy(finale,prima);
printf("Prima stringa tutta in maiuscolo :  %s",finale);
	printf("\nInserisci la seconda stringa: ");
			scanf("%[^\n]%*c",seconda);

int l2=strlen(seconda);
		if(send(CSocket,seconda, l2,0)!= l2){
						Errorhandler("Fail to send().\n ");
						system("pause");
						closesocket(CSocket);
						ClearWinsock();
						return -1;

					}




	if((bytesrcvd=recv(CSocket,seconda,BUFFERSIZE-1,0))<=0){
		Errorhandler("Receive() failed.\n");
		system("pause");
		closesocket(CSocket);
		ClearWinsock();

	}
	seconda[bytesrcvd]='\0';
	strcpy(dfinale,seconda);

	//stampa();
	printf("\nSeconda stringa tutta in minuscolo : %s",seconda);
	//chiudo la connessione

		closesocket(CSocket);
		ClearWinsock();
		printf("\n");
		system("pause");
		return 0;

}

