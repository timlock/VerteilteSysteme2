/* 
 * File:   tcp_server.c
 * Author: H.-J. Eikerling
 * 
 * TCP Echo-Server (einstufig), Version fuer MinGW unter Windows.
 * Verwendet winsock32 (Linker mit -lwsock32 aufrufen).
 * MinGW erlaubt kein fork(), weshalb ein mehrstufiger Server
 * auf diese Art nicht realisiert werden kann.
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
#define MAX_SOCK 10 
#define MAXLINE 512 

/* Vorwaertsdeklarationen */
void str_echo(int);
void err_abort(char *str);

/* Explizite Deklaration zur Vermeidung von Warnungen */
void exit(int code);
void *memset(void *s, int c, size_t n);

int main(int argc, char *argv[]) {

    /* Deskriptoren, Adresslaenge, Prozess-ID */  
    int sockfd, newsockfd, alen, pid;
    int reuse = 1;
    /* Socket Adressen */
    struct sockaddr_in cli_addr, srv_addr;

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
#ifndef __MINGW32__
    /* Nur zum Test: Socketoption zum sofortigen Freigeben der Sockets */
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse)) < 0) {
        err_abort("Kann Socketoption nicht setzen!");
    }
#endif
    /* Binden der lokalen Adresse damit Clients uns erreichen */
    memset((void *) &srv_addr, '\0', sizeof (srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(SRV_PORT);
    if (bind(sockfd, (struct sockaddr *) &srv_addr,
            sizeof (srv_addr)) < 0) {
        err_abort("Kann  lokale  Adresse  nicht  binden,  laeuft  fremder Server?");
    }
    /* Warteschlange fuer TCP-Socket einrichten */
    listen(sockfd, 5);
    printf("TCP Echo-Server: bereit ...\n");

    for (;;) {
        alen = sizeof (cli_addr);

        /* Verbindung aufbauen */ 
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &alen);
        if (newsockfd < 0) {
            err_abort("Fehler beim Verbindungsaufbau!");
        }
        printf ("TCP Echo-Server: neue Verbindung.\n");

#ifdef __MINGW32__
        /* iterativer Server */
        str_echo(newsockfd);
        closesocket(newsockfd);
#else
        /* fuer jede Verbindung einen Kindprozess erzeugen */
        if ((pid = fork()) < 0) {
            err_abort("Fehler beim Erzeugen eines Kindprozesses!");
        } else if (pid == 0) {
            close(sockfd);
            str_echo(newsockfd);
            exit(0);
        }
        close(newsockfd);
#endif
        printf ("TCP Echo-Server: Verbindung beendet.\n");
    }
}

/* str_echo: Lesen von Daten vom Socket und Zuruecksenden an Client */
void str_echo(int sockfd) {
    int n;
    char in[MAXLINE], out[MAXLINE + 6];
    memset((void *) in, '\0', MAXLINE);
    for (;;) {
        /* Daten vom Socket lesen */ 
        // n = read(sockfd, in, MAXLINE);
        n = recv(sockfd, in, MAXLINE, 0);
        if (n == 0) {
            return;
        } else if (n < 0) {
            printf("Fehler beim Lesen des Sockets!");
        }
        printf("%s\n", in);
        sprintf(out, "Echo: %s", in);
        /* Daten schreiben */
        // if (write(sockfd, out, n + 6) != n + 6) {
        if (send(sockfd, out, n + 6, 0) != n + 6) {
            printf("Fehler beim Schreiben des Sockets!");
        }
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
