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
#include <string.h>
#include <stdlib.h> // for atoi()


#define PROTOPORT 27015 // default protocol port number
#define QLEN 6 // size of request queue


void ErrorHandler(char *errorMessage) {
	printf ("%s", errorMessage);
	fflush(stdout);
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




void HandleTCPClient(int clientSocket)
{


	char buffer[512];
	int bytes = recv(clientSocket, buffer, sizeof(buffer)-1, 0);

	if (bytes > 0) {
	    buffer[bytes] = '\0';
	    printf("Ricevuto: %s\n", buffer);
		fflush(stdout);

	    // echo al client
		int j = 0;
		char filtered[bytes];
		for (int i = 0; buffer[i] != '\0'; i++) {
        char c = buffer[i];
        if (!(c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||
              c=='A'||c=='E'||c=='I'||c=='O'||c=='U')) {
            filtered[j++] = c;
        }
    }
    filtered[j] = '\0';

    // invia la stringa senza vocali
    printf("Invio di: %s\n", filtered);
    fflush(stdout);

    send(clientSocket, filtered, strlen(filtered), 0);
    closesocket(clientSocket);
	}

}



int main()
{

	//inizializza winsock
	#if defined WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult != 0) {
		ErrorHandler("Error at WSAStartup()\n");
		return 0;
	}
	else printf ("winsock inizializzato\n");
	#endif


	//crea socket
	int socket = creaSocket();
	if (socket < 0) {
		ErrorHandler("socket creation failed.\n");
		ClearWinSock();
		return -1;
	}
	else printf("Socket creato\n");

	int opt = 1;
	setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

	/// ASSEGNAZIONE DI UN INDIRIZZO ALLA SOCKET
	int port;
	port = PROTOPORT; // use default port number

	char* addr;
	addr = "127.0.0.1";

	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(port); /* converts values between the host and
	network byte order. Specifically, htons() converts 16-bit quantities
	from host byte order to network byte order. */
	if (bind(socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
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


	// SETTAGGIO DELLA SOCKET ALL'ASCOLTO
	if (listen (socket, QLEN) < 0) {
	ErrorHandler("listen() failed.\n");
	closesocket(socket);
	ClearWinSock();
	return -1;
	}
	else ErrorHandler("Socket settato per l'ascolto \n");


	// ACCETTARE UNA NUOVA CONNESSIONE
	struct sockaddr_in cad; // structure for the client address
	int clientSocket; // socket descriptor for the client
	int clientLen; // the size of the client address
	int ascolto = 1;
	ErrorHandler("Waiting for a client to connect...\n");
	while (ascolto) { /* oppure for (;;) */
		clientLen = sizeof(cad); // set the size of the client address
		if ((clientSocket = accept(socket, (struct sockaddr *)&cad, &clientLen)) < 0) {
			ErrorHandler("accept() failed.\n");
			chiudi(socket);
			ClearWinSock();
			return 0;
		}
		char buffer[512];
		int bytes = recv(clientSocket, buffer, sizeof(buffer)-1, 0);
		buffer[bytes] = '\0';
		printf("ricevuti dati dal client (%s) con indirizzo: %s\n", buffer, inet_ntoa(cad.sin_addr));
		fflush(stdout);

		HandleTCPClient(clientSocket);
	}
}



