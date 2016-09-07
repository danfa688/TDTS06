
# Makefile för att kompilera programmet main med g++
#

CCC = g++
CCFLAGS = -g -std=c++11 -pedantic -Wall -Wextra

server: server.o
	$(CCC) $(CCFLAGS) server.o -o server
server.o: server.c
	$(CCC) $(CCFLAGS) -c server.c

# Städa arbetsmappen
clean:
	@ \rm -f *.o
