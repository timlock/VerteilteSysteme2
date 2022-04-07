/* 
 * File:   tcp_client.c
 * Author: H.-J. Eikerling
 * 
 * TCP Echo-Client, Version fuer MinGW unter Windows.
 * Prinzipiell kann auch Telnet als Client verwendet werden. 
 * Allerdings werden Eingaben sofort an den Server uebermittelt
 * und nicht erst nach <RET>.
 * 
 * Verwendet winsock32 (Linker mit -lwsock32 aufrufen).
 *
 * Created on 14. März 2015, 11:03
 */

#include <stdio.h>
#include <string.h> 

#include <stdlib.h>

/* Headerfiles für Netzwerk */
#include <sys/types.h>
#ifdef __MINGW32__
#include <winsock.h>
#else 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
extern pid_t fork(void);
#endif

#define SRV_PORT 8998 
#define MAXLINE 512 

void exit(int code);
void str_client(int);
void err_abort(char *str);

int main(int argc, char *argv[]) {
    /* Deskriptor */
    int sockfd;
    /* Socket Adresse */
    struct sockaddr_in srv_addr, cli_addr;

    /* Argumente testen */
    if (argc != 2) {
        err_abort("IP Adresse des Servers fehlt!");
    }
#ifdef __MINGW32__
    /* Initialisierung */
    int retval;
    WSADATA wsaData;
    if ((retval = WSAStartup(0x202, &wsaData)) != 0) {
        err_abort("WSAStartup() failed!");
    }
#endif 
    /* TCP-Socket erzeugen */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_abort("Kann Stream-Socket nicht oeffnen!");
    }
    /* Adress Struktur fuer Server aufbauen */
    memset((void *) &srv_addr, '\0', sizeof (srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    srv_addr.sin_port = htons(SRV_PORT);

    /* Verbindung aufbauen */
    if (connect(sockfd, (struct sockaddr *) &srv_addr, sizeof (srv_addr)) < 0) {
        err_abort("Fehler beim Verbindungsaufbau!");
    }
    printf("TCP Echo-Client: bereit...\n");

    /* Daten zum Server senden */
    str_client(sockfd);
#ifdef __MINGW32__
    /* iterativer Server */
    closesocket(sockfd);
#else
    close(sockfd);
#endif
    exit(0);
}

/* str_client: Daten von der Standardeingabe lesen, zum Server senden, auf 
das Echo warten und dieses ausgeben */
void str_client(int sockfd) {
    int n;
    char out[MAXLINE], in[MAXLINE + 6];

    /* Lesen bis zum Ende der Eingabe */
    while (fgets(out, MAXLINE, stdin) != NULL) {
        n = strlen(out);
        out[n - 1] = '\0';
        /* Zeile zum Server senden */
        if (send(sockfd, out, n, 0) != n) {
            err_abort("Fehler beim Schreiben des Sockets!");
        }
        /* Echo vom Server lesen */
        n = recv(sockfd, in, MAXLINE, 0);
        if (n < 0) {
            err_abort("Fehler beim Lesen des Sockets!");
        }
        // ausgeben 
        printf("%s\n", in);
    }
}

/* Ausgabe von Fehlermeldungen */
void err_abort(char *str) {
    fprintf(stderr, " TCP Echo-Server: %s\n", str);
    fflush(stdout);
    fflush(stderr);
#ifdef __MINGW32__
    WSACleanup();
#endif
    exit(1);
}

