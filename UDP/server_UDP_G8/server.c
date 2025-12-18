/*
 * server_UDP_G8.c
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

#define PORT 27015
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

    int sock;
    struct sockaddr_in servAddr, clntAddr;
    int clntLen;                  /* Windows: usare int */
    char buffer[BUFSIZE];

    /* CREAZIONE SOCKET UDP */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        ErrorHandler("socket() failed\n");
        return 1;
    }

    /* BIND */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        ErrorHandler("bind() failed\n");
        closesocket(sock);
        ClearWinSock();
        return 1;
    }

    printf("Server UDP in ascolto...\n");
    fflush(stdout);

    while (1) {
        clntLen = sizeof(clntAddr);

        /* 1️⃣ Ricezione "Hello" */
        int bytes = recvfrom(sock, buffer, BUFSIZE - 1, 0,
                             (struct sockaddr*)&clntAddr, &clntLen);
        if (bytes <= 0)
            continue;

        buffer[bytes] = '\0';

        /* DNS inverso */
        struct hostent *he = gethostbyaddr(
            (char*)&clntAddr.sin_addr,
            sizeof(clntAddr.sin_addr),
            AF_INET
        );

        printf("Ricevuti dati dal client nome: %s indirizzo: %s\n",
               he ? he->h_name : "sconosciuto",
               inet_ntoa(clntAddr.sin_addr));
        fflush(stdout);

        /* 2️⃣ Ricezione stringa */
        bytes = recvfrom(sock, buffer, BUFSIZE - 1, 0,
                         (struct sockaddr*)&clntAddr, &clntLen);
        if (bytes <= 0)
            continue;

        buffer[bytes] = '\0';

        printf("Stringa ricevuta: %s\n", buffer);
        fflush(stdout);

        /* 3️⃣ Eliminazione vocali */
        char filtered[BUFSIZE];
        int j = 0;

        for (int i = 0; buffer[i] != '\0'; i++) {
            char c = buffer[i];
            if (!(c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||
                  c=='A'||c=='E'||c=='I'||c=='O'||c=='U')) {
                filtered[j++] = c;
            }
        }
        filtered[j] = '\0';

        /* 4️⃣ Invio risposta */
        sendto(sock, filtered, strlen(filtered), 0,
               (struct sockaddr*)&clntAddr, clntLen);
    }

    closesocket(sock);
    ClearWinSock();
    return 0;
}
