# Makefile fuer TCP Echo Client / Server
# Fuer Kompilierung unter Windows / MinGW Runtime.
# 10/Feb/2017, H.-J. Eikerling

SRVEXE=./tcp_srv.exe
CLTEXE=./tcp_clt.exe

# Auskommentieren, wenn keine Debug-Ausgaben erwuenscht.
#FLAGS=-DDEBUG

CC=gcc
GCC=gcc

all: $(SRVEXE) $(CLTEXE)

$(SRVEXE): tcp_server.o
	$(GCC) -o $(SRVEXE) tcp_server.c -lwsock32 

$(CLTEXE): tcp_client.o
	$(GCC) -o $(CLTEXE) tcp_client.c -lwsock32
	
clean:
	$(RM) *.o $(CLTEXE) $(SRVEXE)

