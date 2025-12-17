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


int initWinsock() {
	#if defined WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult != 0) {
		ErrorHandler("Error at WSAStartup()\n");
		return 0;
	}
	printf ("winsock inizializzato\n");
	return iResult;
	#endif
}


void ClearWinSock() {
	#if defined WIN32
	WSACleanup();
	printf ("winsock pulito\n");
	#endif
}


int creaSocket(){
	int MySocket;
	MySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MySocket < 0) {
		ErrorHandler("socket creation failed.\n");
		ClearWinSock();
		return -1;
	}
	printf("Socket creato\n");
	return MySocket;
}


void chiudi(int socket){

	closesocket(socket);
	printf("Socket chiuso\n");
}




void start(int socket)
{



}



int main() {

	initWinsock();

	int socket = creaSocket();
	start(socket);
	chiudi(socket);

	ClearWinSock();
	return 0;
}



