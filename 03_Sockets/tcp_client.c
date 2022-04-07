/* TCP Echo-Client   
 * Basierf auf Stevens: Unix Network Programming  
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
#define MAXLINE 512 

//void exit(int code); 
void str_client(int); 
void err_abort(char *str); 

int main(int argc, char *argv[]) { 
	// Deskriptor 
	int sockfd; 
	// Socket Adresse 
	struct sockaddr_in srv_addr, cli_addr; 

	// Argumente testen 
	if( argc != 2 ) { 
		err_abort((char*) "IP Adresse des Servers fehlt!"); 
	} 
	// TCP Socket erzeugen 
	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		err_abort((char*) "Kann Stream-Socket nicht oeffnen!"); 
	} 
	// Adress Struktur fuer Server aufbauen 
	memset((void *)&srv_addr, '\0', sizeof(srv_addr)); 
	srv_addr.sin_family = AF_INET; 
	srv_addr.sin_addr.s_addr = inet_addr(argv[1]); 
	srv_addr.sin_port = htons(SRV_PORT); 

	// Verbindung aufbauen 
	if(connect(sockfd,(struct sockaddr *)&srv_addr,sizeof(srv_addr)) < 0){ 
		err_abort((char*) "Fehler beim Verbindungsaufbau!"); 
	} 
	printf((char*) "TCP Echo-Client: bereit...\n"); 

	// Daten zum Server senden 
	str_client(sockfd); 

	close(sockfd); 
	_exit(0); 
} 

/* str_client: Daten von der Standardeingabe lesen, zum Server senden, auf 
das Echo warten und dieses ausgeben */ 
void str_client(int sockfd){ 
	int n; 
	char out[MAXLINE],in[MAXLINE+6]; 

	// Lesen bis zum Ende der Eingabe 
	while(fgets(out,MAXLINE,stdin)!=NULL){ 
		n=strlen(out);
		// Soll eine Zeile geschickt werden, so muss diese mit \n
		// abgeschlossen werden. Wichtig, wenn der Server 
		// versucht, eine Zeile zu lesen. 
		out[n-1]='\n'; 
		// Puffer zum Server senden 
		if(write(sockfd,out,n)!=n){ 
			err_abort((char*) "Fehler beim Schreiben des Sockets!"); 
		} 
		// Echo vom Server lesen 
		n=read(sockfd,in,MAXLINE); 
		if(n<0){ 
			err_abort((char*) "Fehler beim Lesen des Sockets!"); 
		}  
		// ausgeben 
		printf("%s\n",in); 
	} 
} 

/* Ausgabe von Fehlermeldungen */ 
void err_abort(char *str){ 
	fprintf(stderr," TCP Echo-Client: %s\n",str); 
	fflush(stdout); 
	fflush(stderr); 
	_exit(1); 
} 
