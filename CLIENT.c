/*
 * CLIENT.c
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
#include <string.h>
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
	sad.sin_port=htons(27015);


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

	char stringa[]="";
	int x,y;
	printf("\nInserisci la lettera: ");
	scanf("%[^\n]%*c",stringa);
	printf("\nInserisci il primo numero: ");
	scanf("%d",&x);
	printf("\nInserisci il secondo numero: ");
	scanf("%d",&y);
	/*printf(stringa);
	printf("%d",x);
	printf("%d",y);*/

	strcat(stringa,":");
	sprintf(stringa,"%s%d",stringa,x);
	strcat(stringa,":");
	sprintf(stringa,"%s%d",stringa,y);

	//printf(stringa);

	if(send(CSocket,stringa, (strlen(stringa)+2),0)!= (strlen(stringa)+2)){
					Errorhandler("Fail to send().\n ");
					system("pause");
					closesocket(CSocket);
					ClearWinsock();
					return -1;

				}
	//ricevo la stringa risultato
	if((bytesrcvd=recv(CSocket,buf,BUFFERSIZE-1,0))<=0){
					Errorhandler("Receive failed.\n");
					system("pause");
					closesocket(CSocket);
					ClearWinsock();
					return -1;
				}
	buf[bytesrcvd]='\0';   //appendo \0 alla fine della stringa per printarla

	if(strcmp(buf,"TERMINE PROCESSO CLIENT")==0){
		printf("Chiusura connessione..");
		system("pause");
		closesocket(CSocket);
		ClearWinsock();
		return -1;
	}else{
		printf(buf);
	}



//fine programma

	closesocket(CSocket);
			ClearWinsock();
			printf("\n");
			system("pause");
			return 0;

	}
