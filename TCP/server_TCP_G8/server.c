/*
 * server.c
 *
 *  Created on: 11 dic 2025
 *      Author: Danilo Scarpino, Cristina Balestra
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
#include <stdlib.h> // for atoi()





void ErrorHandler(char *errorMessage) {
	printf ("%s", errorMessage);
}




void ClearWinSock() {
	#if defined WIN32
	WSACleanup();
	printf ("winsock pulito\n");
	#endif
}


int creaSocket() {
	int MySocket;
	MySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	return MySocket;
}


void chiudi(int socket) {

	closesocket(socket);
	printf("Socket chiuso\n");
}




void start(int socket)
{
	//assegnaIndirizzo();
}



int main()
{
	#if defined WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	#endif
	if(iResult != 0) {
		ErrorHandler("Error at WSAStartup()\n");
		return 0;
	}
	else printf ("winsock inizializzato\n");


	int socket = creaSocket();
	if (socket < 0) {
		ErrorHandler("socket creation failed.\n");
		ClearWinSock();
		return -1;
	}
	else printf("Socket creato\n");

	start(socket);
	chiudi(socket);

	ClearWinSock();
	return 0;
}



