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


#define BUFFERSIZE 512
#define PROTOPORT 5193 // Numero di porta di defaul


void ErrorHandler(char *errorMessage) {
	printf ("%s", errorMessage);
	fflush(stdout);
}




void ClearWinSock() {
	#if defined WIN32
	WSACleanup();
	ErrorHandler ("winsock pulito\n");
	#endif
}


int creaSocket() {
	int MySocket;
	MySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	return MySocket;
}


void chiudi(int socket) {

	closesocket(socket);
	ErrorHandler("Socket chiuso\n");
}

int main(void)
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
	else ErrorHandler("winsock inizializzato\n");


	//crea socket
	int socket = creaSocket();
	if (socket < 0) {
		ErrorHandler("socket creation failed.\n");
		chiudi(socket);
		ClearWinSock();
		return -1;
	}
	else ErrorHandler("Socket creato\n");


	// COSTRUZIONE DELL’INDIRIZZO DEL SERVER
	char* addr;
	addr = "127.0.0.1";
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr(addr); // IP del server
	sad.sin_port = htons(27015); // Server port


	// CONNESSIONE AL SERVER
	if (connect(socket, (struct sockaddr *)&sad, sizeof(sad)) < 0)
	{
	ErrorHandler("Failed to connect.\n");
	chiudi(socket);
	ClearWinSock();
	return -1;
	}

	char* handshake = "Hello"; // Stringa da inviare
	int stringLen = strlen(handshake);  // Determina la lunghezza

	// INVIARE DATI AL SERVER
	if (send(socket, handshake, stringLen, 0) != stringLen) {
	ErrorHandler("send() sent a different number of bytes than expected");
	chiudi(socket);
	ClearWinSock();
	return -1;
	}

	// INPUT DA TASTIERA
	char inputString[BUFFERSIZE];

	ErrorHandler("Inserisci una stringa: ");

	if (fgets(inputString, BUFFERSIZE, stdin) == NULL) {
		ErrorHandler("Errore lettura input\n");
		chiudi(socket);
		ClearWinSock();
		return -1;
	}

	// INVIARE DATI AL SERVER
	stringLen = strlen(inputString);
	if (send(socket, inputString, stringLen, 0) != stringLen) {
		    ErrorHandler("send() failed\n");
		    chiudi(socket);
		    ClearWinSock();
		    return -1;
		}



	// RICEVERE DATI DAL SERVER
	int bytesRcvd;
	int totalBytesRcvd = 0;
	char buf[BUFFERSIZE];   // buffer for data from the server
	ErrorHandler("Received: ");   // Setup to print the echoed string

	while (totalBytesRcvd < stringLen) {
	if ((bytesRcvd = recv(socket, buf, BUFFERSIZE - 1, 0)) <= 0) {
	ErrorHandler("recv() failed or connection closed prematurely");
	chiudi(socket);
	ClearWinSock();
	return -1;
	}
	totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
	buf[bytesRcvd] = '\0'; // Add \0 so printf knows where to stop
	ErrorHandler(buf); // Print the echo buffer
	}

	chiudi(socket);

	ClearWinSock();
	return 0;
}

