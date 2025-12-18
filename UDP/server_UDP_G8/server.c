/*
 * client.c
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

#define ECHOMAX 255
#define PORT 48000



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
	MySocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	return MySocket;
}


void chiudi(int socket) {

	closesocket(socket);
	printf("Socket chiuso\n");
}




int main()
{
	//inizializza winsock
	#if defined WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	#endif
	if(iResult != 0) {
		ErrorHandler("Error at WSAStartup()\n");
		return 0;
	}
	else printf ("winsock inizializzato\n");


	//crea socket
	unsigned int cliAddrLen;
	char echoBuffer[ECHOMAX];
	int recvMsgSize;
	int socket = creaSocket();
	if (socket < 0) {
		ErrorHandler("socket creation failed.\n");
		ClearWinSock();
		return -1;
	}
	else printf("Socket creato\n");

	/// ASSEGNAZIONE DI UN INDIRIZZO ALLA SOCKET
	int port;
	port = PORT; // use default port number
	char* addr;
	addr = "127.0.0.1";
	struct sockaddr_in echoServAddr;
	memset(&echoServAddr, 0, sizeof(echoServAddr)); // ensures that extra bytes contain 0
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_port = htons(port);
	echoServAddr.sin_addr.s_addr = inet_addr(addr);

	struct sockaddr_in sad; /* converts values between the host and
	network byte order. Specifically, htons() converts 16-bit quantities
	from host byte order to network byte order. */
	if ((bind(socket, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr))) < 0) {
	printf("bind() failed: %d\n", WSAGetLastError());
	closesocket(socket);
	ClearWinSock();
	return -1;
	}
	else
		{
		char err_buffer[100];
		sprintf(err_buffer, "Socket assegnato a %s:%d\n", addr, port);
		ErrorHandler(err_buffer);
		}


	// RICEZIONE DELLA STRINGA ECHO DAL CLIENT
	struct sockaddr_in echoClntAddr;
	while(1) {
	cliAddrLen = sizeof(echoClntAddr);
	recvMsgSize = recvfrom(socket, echoBuffer, ECHOMAX, 0, (struct
	sockaddr*)&echoClntAddr, &cliAddrLen);
	printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
	printf("Received: %s\n", echoBuffer);

	// RINVIA LA STRINGA ECHO AL CLIENT
	if (sendto(socket, echoBuffer, recvMsgSize, 0, (struct sockaddr *)&echoClntAddr,
	sizeof(echoClntAddr)) != recvMsgSize)
	ErrorHandler("sendto() sent different number of bytes than expected");
	}

	chiudi(socket);

	ClearWinSock();
	return 0;
}
