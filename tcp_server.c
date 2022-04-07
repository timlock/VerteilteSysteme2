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
#include <sys/stat.h>
#include <malloc.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>

//#define SRV_PORT 8888
int SRV_PORT = -1;
#define MAX_SOCK 10
#define MAXLINE 512

// Vorwaertsdeklarationen
void str_web(int);

void err_abort(char *str);

// Explizite Deklaration zur Vermeidung von Warnungen
//void exit(int code);
void *memset(void *s, int c, size_t n);

char *workspace;


char htmlHeader[] = "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html; charset=iso-8859-1\r\n\r\n";

char jpgHeader[] = "HTTP/1.1 200 OK\r\n"
                   "Content-Type: image/jpg;\r\n"
                   "charset=iso-8859-1\r\n\r\n";

char failHeader[] = "HTTP/1.1 404 NOT OK\r\n"
                    "Content-Type: text/html; charset=iso-8859-1\r\n\r\n"
                    "<html>404</html\r\n";

void writeToStream(int sockfd, char *buffer, size_t size);

char *indexGenerator(char *fileName);

bool isJPG(char *path);

<<<<<<< HEAD

bool isHTML(char *path);

=======
>>>>>>> 528dfb0dc798cb55b4933e687ee990375fe84207
int main(int argc, char *argv[]) {
    FILE *file = fopen("erstellt", "r");

    // If file is NULL return
    if (!file) {
        return 1;
    }

    // Array to store the char bytes
    unsigned char bytes[3];

    // Read the file bytes
    fread(bytes, 3, 1, file);

    // Condition for JPEG image

    // If the given file is a JPEG file
    if (bytes[0] == 0xff
        && bytes[1] == 0xd8
        && bytes[2] == 0xff) {

        printf("This Image is "
               "in JPEG format!");
    }

        // Else print "No"
    else {

        printf("No");
    }

    return 0;


    if (argc < 2) {
        err_abort((char *) "Es muss ein Verzeichnis und ein Port übergeben werden\ntcp_server <workspace> <port>\n");
    }

    workspace = argv[1];
    SRV_PORT = atoi(argv[2]);
    printf("Workspace: %s\n", workspace);
    printf("Port: %d\n", SRV_PORT);
    // Deskriptoren, Adresslaenge, Prozess-ID
    int sockfd, newsockfd, pid;
    socklen_t alen;
    int reuse = 1;
    // Socket Adressen
    struct sockaddr_in cli_addr, srv_addr;

    // TCP-Socket erzeugen
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_abort((char *) "Kann Stream-Socket nicht oeffnen!");
    }
    // Nur zum Test: Socketoption zum sofortigen Freigeben der Sockets
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        err_abort((char *) "Kann Socketoption nicht setzen!");
    }
    // Binden der lokalen Adresse damit Clients uns erreichen
    memset((void *) &srv_addr, '\0', sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(SRV_PORT);
    if (bind(sockfd, (struct sockaddr *) &srv_addr,
             sizeof(srv_addr)) < 0) {
        err_abort((char *) "Kann  lokale  Adresse  nicht  binden,  laeuft  fremder Server?");
    }
    // Warteschlange fuer TCP-Socket einrichten
    listen(sockfd, 5);
    char *serverIp = inet_ntoa(srv_addr.sin_addr);
    printf((char *) "TCP Web-Server %s : bereit ...\n", serverIp);

    for (;;) {
        alen = sizeof(cli_addr);

        // Verbindung aufbauen
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &alen);
        if (newsockfd < 0) {
            err_abort((char *) "Fehler beim Verbindungsaufbau!");
        }
        char *clientIp = inet_ntoa(cli_addr.sin_addr);

        // fuer jede Verbindung einen Kindprozess erzeugen
        if ((pid = fork()) < 0) {
            err_abort((char *) "Fehler beim Erzeugen eines Kindprozesses!");
        } else if (pid == 0) {
            close(sockfd);
            str_web(newsockfd);
            _exit(0);
        }
        close(newsockfd);

    }
}

void str_web(int sockfd) {
    int n;
    char in[MAXLINE], out[MAXLINE + 6];
    memset((void *) in, '\0', MAXLINE);
    char* currentPwd;
    for (;;) {
        // Daten vom Socket lesen
        n = read(sockfd, in, MAXLINE);
        if (n == 0) {
            return;
        } else if (n < 0) {
            err_abort((char *) "Fehler beim Lesen des Sockets!");
        }
        printf((char *) "Get Request erhalten\n %s\n", in);

        //  Angeforderte Datei bestimmen
        char *header = NULL;
<<<<<<< HEAD

        char * requiredFile = strtok(in," ");
        printf("RequiredFIle: %s\n", requiredFile);
        requiredFile = strtok(NULL," ");
        printf("RequiredFIle: %s\n", requiredFile);
//        requiredFile++;

//        char url[255];
//        sscanf(in, "GET %255s HTTP/", url);
//        char *tmp = url;
//        tmp += 1;
//        char* requiredFile = strtok(tmp,"\0");
//        if(requiredFile == '\0') requiredFile =" " ;

=======
        char *path = NULL;
        char url[255];
        sscanf(in, "GET %255s HTTP/", url);
        char *requiredFile = url;
        requiredFile +=1;
>>>>>>> 528dfb0dc798cb55b4933e687ee990375fe84207
        printf("Angeforderte Datei: %s\n", requiredFile);

        /*
        if (strcmp(requiredFile, "/") == 0 || strcmp(requiredFile, "/der_werwolf.html") == 0) {
            char werwolf[] = "der_werwolf.html";
            header = htmlHeader;
            path =  (char*)  malloc(sizeof workspace + sizeof werwolf);
            strcat(path, workspace);
            strcat(path, werwolf);
        } else if (strcmp(requiredFile, "/C_Morgenstern.jpg") == 0) {
            char morgenstern[] = "C_Morgenstern.jpg";
            header = jpgHeader;
            path = (char*) malloc(sizeof workspace + sizeof morgenstern);
            strcat(path, workspace);
            strcat(path, morgenstern);
        }
        */

<<<<<<< HEAD

        printf("Path size%zu\n", sizeof (workspace) + sizeof (requiredFile));
        char *path = (char *) malloc(sizeof (workspace) + sizeof (requiredFile));

=======
        path = (char *) malloc(strlen(workspace) + strlen(requiredFile));
>>>>>>> 528dfb0dc798cb55b4933e687ee990375fe84207
        strcat(path, workspace);
        strcat(path, requiredFile);
        struct stat fileInfo;
        stat(path, &fileInfo);

        mode_t m = fileInfo.st_mode;

        bool isDir = false;

        if (strstr(requiredFile, ".html") != NULL) {
            header = htmlHeader;
            printf("Html: %s\n", requiredFile);
        } else if (strstr(requiredFile, ".jpg") != NULL) {
            header = jpgHeader;
            printf("Jpg: %s\n", requiredFile);
        } else if (S_ISDIR(m) != 0) {
            header = htmlHeader;
            printf("Dir: %s\n", requiredFile);
            isDir = true;
        }else{
            header = failHeader;
        }

        printf("Path: %s\n", path);

<<<<<<< HEAD

=======
        //  Datei öffnen
        printf("Size %d\n",strlen(path));
        FILE * file = fopen(path, "rb");
        if (!file) {
            printf("Nicht vorhandene Datei angefordert, %s\n", path);
            write(sockfd, failHeader, sizeof(failHeader) - 1);
            printf("Size %d\n",strlen(path));
            return;
        }
>>>>>>> 528dfb0dc798cb55b4933e687ee990375fe84207
        writeToStream(sockfd, header, strlen(header));

        //Datei Größe bestimmen
        size_t fileSize;
        char *indexHtml = NULL;
        FILE *file;
        if (isDir) {
            indexHtml = indexGenerator(path);
            fileSize = strlen(indexHtml);
        } else {
            //  Datei öffnen
             file = fopen(path, "rb");
            if (!file) {
                printf("Nicht vorhandene Datei angefordert, %s\n", path);
                write(sockfd, failHeader, sizeof(failHeader) - 1);
                return;
            }
            fileSize = fileInfo.st_size;
        }
        printf("Dategroesse %zu\n", fileSize);
        char body[fileSize];
        //  Body einlesen
        if (isDir) {
            writeToStream(sockfd, indexHtml, fileSize);
        } else {
            fread(body, 1, fileSize, file);
            body[fileSize] = '\0';
            writeToStream(sockfd, body, fileSize);
        }

        free(indexHtml);
        free(path);
        if(!isDir)fclose(file);
        printf("Fertig\n");
        return;
    }
}

void writeToStream(int sockfd, char *buffer, size_t fileSize) {
    size_t remainingBytes = fileSize;
<<<<<<< HEAD

=======
   // printf("writeToStream:\n%s\n", buffer);
>>>>>>> 528dfb0dc798cb55b4933e687ee990375fe84207
    while (remainingBytes > 0) {
        size_t sendBytes = write(sockfd, buffer, remainingBytes);
        remainingBytes -= sendBytes;
        printf("Bytes gesendet: %zu, verbleibende Bytes %zu\n", sendBytes, remainingBytes);
    }
}

<<<<<<< HEAD

char *indexGenerator(char *fileName) {
    char *fullPath = realpath(fileName, NULL);
=======
char *indexGenerator(char *fileName) {
    char * fullPath = realpath(fileName, NULL);
>>>>>>> 528dfb0dc798cb55b4933e687ee990375fe84207
    fullPath[strlen(fullPath)] = '/';
    printf("Fullpath %s\n", fullPath);
    char *htmlAnfang = "<html>\n"
                       "<head>\n"
                       "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n"
                       "<title>Verlinkungen zum Testen</title>\n"
                       "<style>\n"
                       "h2 {\n"
                       "font-family: \"Arial\", Arial, sans-serif;\n"
                       "}\n"
                       "table {\n"
                       "border-spacing: 15px 0;\n"
                       "font-family: \"Lucida Console\", Lucida, monospace;\n"
                       "}\n"
                       "</style>\n"
                       "</head>\n"
                       "<body>\n"
                       "<font face=\"arial\"></font>\n"
                       "<h2>Verlinkungen zum Testen</h2>\n"
                       "<table>";

    char *tmp1 = "<tr>\n"
                 "<td><a href=\"";
    char *tmp2 = "\">";
    char *tmp3 = "</a></td>\n"
                 "</tr>\n";

    char *htmlEnde = "</table>\n"
                     "</body>\n"
                     "</html>\0";

    struct dirent *dp;

    DIR *dir = opendir(fileName);

    if (!dir) {
        printf("Verzeichnis %s konnte nicht geoeffnet werden\n", fileName);
    }

    unsigned int newLength = 0;

    while (dp = readdir(dir)) // Zum Bestimmen der Laenge der Dateien im Verzeichnis und dem gesamten <tr> tag
    {
<<<<<<< HEAD

        char *path = strstr(fullPath, workspace);
        path += strlen(workspace);
        char *relativePath = malloc(strlen(path) + strlen(dp->d_name));
        strcat(relativePath, path);
        strcat(relativePath, dp->d_name);
        newLength += (strlen(tmp1) + strlen(tmp2) + strlen(tmp3) + +strlen(fileName) + strlen(relativePath));
        free(relativePath);

=======
        char *path = strstr(fullPath, workspace);
        path += strlen(workspace);
        printf("Index link für %s ist %s\n", dp->d_name, path);
        char * relativePath = malloc(strlen(path) + strlen(dp->d_name));
        strcat(relativePath, path);
        strcat(relativePath, dp->d_name);
        printf("Relativer Pfad ist %s\n", relativePath);
        newLength += (strlen(tmp1) + strlen(tmp2) + strlen(tmp3) + +strlen(fileName)+ strlen(relativePath));
        free(relativePath);
>>>>>>> 528dfb0dc798cb55b4933e687ee990375fe84207
    }

    char *ergebnis = (char *) malloc(strlen(htmlAnfang) + strlen(htmlEnde) + newLength + 1);
    strcat(ergebnis, htmlAnfang);
    rewinddir(dir);

    while (dp = readdir(dir)) // Zum Verketten des <tr> tags mit den Verzeichnisdateien
    {
<<<<<<< HEAD

        char *path = strstr(fullPath, workspace);
        path += strlen(workspace);
        char *relativePath = malloc(strlen(path) + strlen(dp->d_name));
        strcat(relativePath, path);
        strcat(relativePath, dp->d_name);

=======
        char *path = strstr(fullPath, workspace);
        path += strlen(workspace);
        char * relativePath = malloc(strlen(path) + strlen(dp->d_name));
        strcat(relativePath, path);
        strcat(relativePath, dp->d_name);
>>>>>>> 528dfb0dc798cb55b4933e687ee990375fe84207
        strcat(ergebnis, tmp1);
        strcat(ergebnis, relativePath);
        strcat(ergebnis, tmp2);
        strcat(ergebnis, dp->d_name);
        strcat(ergebnis, tmp3);
        free(relativePath);
    }

    closedir(dir);

    strcat(ergebnis, htmlEnde);

    return ergebnis;
}

bool isJPG(char *path) {
    FILE *file = fopen(path, "r");

    if (!file) {
        printf("Datei konnte nicht geoeffnet werden");
        return false;
    }
    unsigned char bytes[3];
    fread(bytes, 3, 1, file);
    // If the given file is a JPEG file
    if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff) {
        fclose(file);
        return true;
    }
    fclose(file);
    return false;
}

/* Ausgabe von Fehlermeldungen */
void err_abort(char *str) {
    fprintf(stderr, " TCP Echo-Server: %s\n", str);
    fflush(stdout);
    fflush(stderr);
    _exit(1);
}
