# Makefile fuer TCP Client / Server
# 14/Mar/2021, H.-J. Eikerling

SRVEXE=./tcp_srv
CLTEXE=./tcp_clt

# Auskommentieren, wenn keine Debug-Ausgaben erwuenscht.
#FLAGS=-DDEBUG

GCC=g++
CC=$(GCC)

all: $(SRVEXE) $(CLTEXE)

$(SRVEXE): tcp_server.o
	$(GCC) -o $(SRVEXE) tcp_server.c

$(CLTEXE): tcp_client.o
	$(GCC) -o $(CLTEXE) tcp_client.c
	
clean:
	$(RM) *.o $(CLTEXE) $(SRVEXE)

