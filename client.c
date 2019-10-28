/*
 * client.c
 *
 *  Created on: 19/ott/2019
 *      Author: Danilo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <netdb.h>
#endif
#define BUFFERSIZE 512

void HandlingServer(int CSocket);

void Clearwinsock(){
#if defined WIN32
	WSACleanup();
#endif
}

int main(){

#if defined WIN32
	int iResult;
	WSADATA WsaData;
	iResult=WSAStartup(MAKEWORD(2,2), &WsaData);
	if(iResult!=0){
		printf("Error at WSAStartup().\n");
		system("pause");
		return EXIT_FAILURE;
	}
#endif
//richiedo nome simbolico del server
	char name[]="";
	struct hostent *host;
	printf("Inserisci il nome del server a cui devo connettermi: ");
	scanf("%s",name);

	host=gethostbyname(name);

	if(host==NULL){
		fprintf(stderr,"gethostbyname() failed.\n");
		system("pause");
		Clearwinsock();
		return EXIT_FAILURE;
	}

//creazione socket

	int CSocket;
	CSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(CSocket<0){
		fprintf(stderr,"Creazione socket fallita.\n");
		system("pause");
		Clearwinsock();
		return EXIT_FAILURE;
	}

// assegnazione indirizzo server
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	//memcpy(&serveraddr.sin_addr,host->h_addr_list[0],host->h_length);
	struct in_addr* ina=(struct in_addr*)host->h_addr_list[0];
	serveraddr.sin_addr=*ina;
	serveraddr.sin_port=htons(27015);

	printf("Risultato di gethostbyname(%s) : %s\n",name,inet_ntoa(serveraddr.sin_addr));

	if(connect(CSocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
		printf("Fail to connect().\n");
		system("pause");
		Clearwinsock();
		return -1;

	}

	HandlingServer(CSocket);


Clearwinsock();
closesocket(CSocket);
system("pause");
return 0;
}//fine main



void HandlingServer(int CSocket){
	//ricevo la stringa di connessione avvenuta

	char buf[512]="c";
	int bytesrcvd;

	if((bytesrcvd=recv(CSocket,buf,BUFFERSIZE-1,0))<=0){
				printf("Receive failed.\n");
				system("pause");
				Clearwinsock();

			}

	buf[bytesrcvd]='\0';   //appendo \0 alla fine della stringa per printarla
	printf("%s\n",buf);     //print the echo buffer

	//prendo in input 2 stringhe e le mando al server
	char prim[512]="c";
	char second[512]="c";


			printf("Inserisci la prima stringa: ");
			gets(prim);
			scanf("%[^\n]%*c",prim);
			strcat(prim,":");

			printf("\nInserisci la seconda stringa: ");
				//gets(seconda);
			scanf("%[^\n]%*c",second);

			strcat(prim,second);

			int l1=strlen(prim);

			if(send(CSocket,prim, l1+1,0)!= l1+1){
					printf("Fail to send().\n ");
					system("pause");
					Clearwinsock();

						}
//ricevo le stringhe modificate dal server

		if((bytesrcvd=recv(CSocket,buf,BUFFERSIZE-1,0))<=0){
				printf("Receive failed.\n");
				system("pause");
				Clearwinsock();
					}
		char *token=strtok(buf,":");
		strcpy(prim,token);
		token=strtok(NULL,":");
		strcpy(second,token);
		strcat(prim,"\0");//appendo \0 alla fine della stringa per printarla
		printf("%s\n",prim);     //print the echo buffer
		strcat(second,"\0");
		printf("%s\n",second);

}



