/*
 * client.c
 *
 *  Created on: 11 dic 2025
 *      Author: Danilo Scarpino, Cristina Balestra
 */


#include "client.h"
int main() {

	WSADATA wsaData;
	WORD wVersionRequested;
	wVersionRequested= MAKEWORD(2,2);
	int iResult = WSAStartup(wVersionRequested, &wsaData);
	if (iResult != 0) {
		printf("Error at WSAStartup()\n");
		printf("A usable WinSock DLL cannot be found");
		return-1;
	}
	// The WinSock DLL is acceptable. Proceed
	printf("No errors occurred. \n");
	system("pause");
	return 0;
}
