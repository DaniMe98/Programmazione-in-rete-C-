/*
 * server.c
 *
 *  Created on: 24/ott/2019
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
int MySocket;
void HandlingClient(int c,char* a,int x,int y);


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

	//primo punto (invio al client il mex di connessione)
		char str[]="connessione avvenuta";

		int strl=strlen(str);


		if(send(ClientSocket,str, strl,0)!= strl){
				ErrorHandler("Fail to send().\n ");
				system("pause");
				closesocket(MySocket);
				ClearWinSock();
				return -1;
				}
	//ricevo la stringa dal client e la processo
		if((bytesrcvd=recv(ClientSocket,buf,511,0))<=0){
									ErrorHandler("Receive failed.\n");
									system("pause");
									closesocket(MySocket);
									ClearWinSock();
							 		return -1;
								}
	/*	buf[bytesrcvd]='\0';
		printf("%s",buf);*/
		char *token=strtok(buf,":");
		char prima[1];
		strcpy(prima,token);
		strcat(prima,"\0");
		token=strtok(NULL,":");
		int seconda=atoi(token);
		token=strtok(NULL,":");
		int y=atoi(token);


		HandlingClient(ClientSocket,prima,seconda,y);

	}


	//fine programma
		system("pause");
		closesocket(ClientSocket);
		return 0;
}


void HandlingClient(int c,char* a,int x,int y){
	int r;
	char risultato[50];
	strcpy(risultato,strupr(a));
	switch(risultato[0]){

				case 'A':{
					r=x+y;
					sprintf(risultato,"Risultato dell'addizione: %d",r);

				}
				break;
				case 'M':{
					r=x*y;
					sprintf(risultato,"Risultato della moltiplicazione: %d",r);

				}
				break;
				case 'S':{
					r=x-y;
					sprintf(risultato,"Risultato della sottrazione: %d",r);

				}
				break;
				case 'D':{
					r=x/y;
					sprintf(risultato,"Risultato della divisione: %d",r);

				}
				break;
				default:{
					sprintf(risultato,"TERMINE PROCESSO CLIENT");
				}
				break;

	}

	if(send(c,risultato,strlen(risultato),0)!=strlen(risultato)){
		ErrorHandler("Fail to send().\n ");
		system("pause");
		closesocket(MySocket);
		ClearWinSock();

	}


}

