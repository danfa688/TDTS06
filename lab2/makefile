
# Makefile för att kompilera programmet main med g++
#

CCC = g++
CCFLAGS = -g -std=c++11 -pedantic -Wall -Wextra

all: server

server: server.o client.o util.o
	$(CCC) $(CCFLAGS) server.o client.o util.o -o server
server.o: server.c
	$(CCC) $(CCFLAGS) -c server.c
client.o: client.c
	$(CCC) $(CCFLAGS) -c client.c
util.o: util.c
	$(CCC) $(CCFLAGS) -c util.c

# Städa arbetsmappen
clean:
	@ \rm -f *.o
