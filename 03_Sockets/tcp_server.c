/* Mehrstufiger TCP Echo-Server fuer mehrere Clients  
 * Basiert auf Stevens: Unix Network Programming  
 * getestet unter Ubuntu 20.04 64 Bit 
 */ 

#include <stdio.h> 
#include <string.h> 
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 

#define SRV_PORT 8998 
#define MAX_SOCK 10 
#define MAXLINE 512 

// Vorwaertsdeklarationen 
void str_echo(int);  
void err_abort(char *str); 

// Explizite Deklaration zur Vermeidung von Warnungen 
//void exit(int code);
void *memset(void *s, int c, size_t n); 

int main(int argc, char *argv[]) { 

	// Deskriptoren, Adresslaenge, Prozess-ID  
	int sockfd, newsockfd, pid; 
	socklen_t alen; 
	int reuse = 1; 
	// Socket Adressen 
	struct sockaddr_in cli_addr, srv_addr; 

	// TCP-Socket erzeugen 
	if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		err_abort((char*) "Kann Stream-Socket nicht oeffnen!"); 
	} 
	// Nur zum Test: Socketoption zum sofortigen Freigeben der Sockets 
	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))<0){ 
		err_abort((char*) "Kann Socketoption nicht setzen!"); 
	} 
	// Binden der lokalen Adresse damit Clients uns erreichen 
	memset((void *)&srv_addr, '\0', sizeof(srv_addr)); 
	srv_addr.sin_family = AF_INET; 
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	srv_addr.sin_port = htons(SRV_PORT); 
	if(bind(sockfd, (struct sockaddr *)&srv_addr, 
		sizeof(srv_addr)) < 0 ) { 
			err_abort((char*) "Kann  lokale  Adresse  nicht  binden,  laeuft  fremder Server?"); 
	} 
	// Warteschlange fuer TCP-Socket einrichten 
	listen(sockfd,5); 
	printf((char*) "TCP Echo-Server: bereit ...\n"); 

	for(;;){ 
		alen = sizeof(cli_addr); 

		// Verbindung aufbauen 
		newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr,&alen); 
		if(newsockfd < 0){ 
			err_abort((char*) "Fehler beim Verbindungsaufbau!"); 
		} 

		// fuer jede Verbindung einen Kindprozess erzeugen 
		if((pid = fork()) < 0){ 
			err_abort((char*) "Fehler beim Erzeugen eines Kindprozesses!"); 
		} else if (pid == 0){ 
			close (sockfd); 
			str_echo (newsockfd); 
			_exit (0); 
		} 
		close (newsockfd); 
	} 
}  

/* str_echo: Lesen von Daten vom Socket und Zuruecksenden an Client */ 
void str_echo(int sockfd) { 
	int n; 
	char in[MAXLINE], out[MAXLINE+6]; 
	memset ((void *)in,'\0',MAXLINE); 
	for(;;){ 
		// Daten vom Socket lesen 
		n = read (sockfd,in,MAXLINE); 
		if (n == 0){ 
			return; 
		} else if (n < 0){ 
			err_abort((char*) "Fehler beim Lesen des Sockets!"); 
		} 
		printf ("%s\n", in);
		sprintf (out, "Echo: %s", in); 
		// Daten schreiben 
		if(write (sockfd, out, n+6) != n+6){ 
			err_abort ((char*) "Fehler beim Schreiben des Sockets!"); 
		} 
	} 
} 

/* Ausgabe von Fehlermeldungen */ 
void err_abort(char *str){ 
	fprintf(stderr," TCP Echo-Server: %s\n",str); 
	fflush(stdout); 
	fflush(stderr); 
	_exit(1); 
} 
