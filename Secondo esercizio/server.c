/*
 * server.c
 *
 *  Created on: 19/ott/2019
 *      Author: Danilo
 */

#if defined WIN32
#include <winsock.h>
#else
#include <netdb.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#define QLEN 6

void Clearwinsock(){
#if defined WIN32
	WSACleanup();
#endif
}

void HandlingClient(int c,int d);

int main(){

#if defined WIN32
	int iResult;
		WSADATA WsaData;
		iResult=WSAStartup(MAKEWORD(2,2), &WsaData);
		if(iResult!=0){
			printf("Error at WSAStartup().\n");
			return 0;
		}
#endif

//creazione della socket
		int SerSocket;

		SerSocket=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(SerSocket<0){
			printf("Error in creating socket.\n");
			system("pause");
			Clearwinsock();
			return 0;
		}
//assegnazione di un indirizzo alla socket

		struct sockaddr_in sad;
		memset(&sad,0,sizeof(sad));
		sad.sin_family=AF_INET;
		sad.sin_addr.s_addr=inet_addr("172.16.1.224");
		sad.sin_port=htons(27015);

//bind()
		if(bind(SerSocket,(struct sockaddr*)&sad ,sizeof(sad))<0){
			printf("Error in bind().\n");
			system("pause");
			Clearwinsock();
			return -1;
		}
//settaggio socket in ascolto
		if(listen(SerSocket,QLEN)<0){
			printf("Error in listen().\n");
						system("pause");
						Clearwinsock();
						return -1;
		}

//accettiamo una nuova connesione

		struct sockaddr_in cad; //(client address)
		int ClientSocket;
		int ClientLen; //lunghezza dell'indirizzo del client

		printf("Waiting for a client to connect...");
		while(1){
				ClientLen=sizeof(cad);
				ClientSocket=accept(SerSocket, (struct sockaddr*) &cad, &ClientLen);
				if(ClientSocket<0){
					printf("accept() Failed.\n");
					system("pause");
					Clearwinsock();
					return EXIT_FAILURE;
					}
				struct in_addr addr;
				addr.s_addr=inet_addr(inet_ntoa(cad.sin_addr));
				struct hostent* host;
				host=gethostbyaddr((char *)&addr,4,AF_INET);
				char* canonicalname=host->h_name;
				printf("\nHandling client: ip(%s) name(%s).\n",inet_ntoa(cad.sin_addr),canonicalname);

				HandlingClient(ClientSocket,SerSocket);

		}//while(1)
		system("pause");
		return 0;
}//main


void HandlingClient(int ClientSocket,int ServerSocket){
	char str[]="connessione avvenuta";

			int strl=strlen(str);

		//primo punto (invio al client il mex di connessione)

			if(send(ClientSocket,str, strl,0)!= strl){
						printf("Fail to send().\n ");
						system("pause");
						Clearwinsock();
						}

		//ricevo la stringa concatenata dal client e la divido e stampo
			char buf[512];
		int bytesrcvd;
		if((bytesrcvd=recv(ClientSocket,buf,511,0))<=0){
						printf("Receive failed.\n");
						system("pause");
						Clearwinsock();
					}

		char *token=strtok(buf,":");
		char prima[100];
		strcpy(prima,token);
		token=strtok(NULL,":");
		char seconda[100];
		strcpy(seconda,token);


		strcat(prima,"\0");//appendo \0 alla fine della stringa per printarla
		printf("%s\n",prima);     //print the echo buffer
		strcat(seconda,"\0");
		printf("%s\n",seconda);
		strupr(prima);
		strlwr(seconda);
		strcat(prima,":");
		strcat(prima,seconda);
		//printf("%s\n",prima);

		strl=strlen(prima);
		if(send(ClientSocket,prima, strl+1,0)!= strl+1){
								printf("Fail to send().\n ");
								system("pause");
								Clearwinsock();
								}


}//fine handling




















