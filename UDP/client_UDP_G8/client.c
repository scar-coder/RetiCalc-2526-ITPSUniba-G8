/*
 * client_UDP_G8.c
 *
 *  Created on: 11 dic 2025
 *  Author: Danilo Scarpino, Cristina Balestra
 */

#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 512

void ErrorHandler(char *msg) {
    printf("%s", msg);
    fflush(stdout);
}

void ClearWinSock() {
#if defined WIN32
    WSACleanup();
#endif
}

int main() {

#if defined WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        ErrorHandler("WSAStartup failed\n");
        return 1;
    }
#endif

    char host[100];
    int port;
    char buffer[BUFSIZE];

    /* INPUT HOST */
    printf("Inserisci nome host del server: ");
    fflush(stdout);
    scanf("%99s", host);

    /* INPUT PORTA */
    printf("Inserisci porta del server: ");
    fflush(stdout);
    scanf("%d", &port);

    /* DNS */
    struct hostent *he = gethostbyname(host);
    if (he == NULL) {
        ErrorHandler("Errore DNS\n");
        return 1;
    }

    int sock;
    struct sockaddr_in servAddr, fromAddr;
    int fromLen = sizeof(fromAddr);

    /* SOCKET UDP */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        ErrorHandler("socket() failed\n");
        return 1;
    }

    /* INDIRIZZO SERVER */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    memcpy(&servAddr.sin_addr, he->h_addr_list[0], he->h_length);

    /* INVIO HELLO */
    sendto(sock, "Hello", 5, 0,
           (struct sockaddr*)&servAddr, sizeof(servAddr));

    /* INPUT STRINGA */
    printf("Inserisci stringa: ");
    fflush(stdout);
    scanf("%511s", buffer);

    /* INVIO STRINGA */
    sendto(sock, buffer, strlen(buffer), 0,
           (struct sockaddr*)&servAddr, sizeof(servAddr));

    /* RICEZIONE RISPOSTA */
    int bytes = recvfrom(sock, buffer, BUFSIZE - 1, 0,
                         (struct sockaddr*)&fromAddr, &fromLen);

    if (bytes < 0) {
        ErrorHandler("recvfrom() failed\n");
        closesocket(sock);
        ClearWinSock();
        return 1;
    }

    buffer[bytes] = '\0';

    printf("Stringa %s ricevuta dal server nome:%s indirizzo:%s\n",
           buffer,
           he->h_name,
           inet_ntoa(fromAddr.sin_addr));

    closesocket(sock);
    ClearWinSock();
    return 0;
}
