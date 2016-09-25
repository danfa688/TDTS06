
# Makefile för att kompilera programmet main med g++
#

CCC = g++
CCFLAGS = -g -std=c++11 -pedantic

all: main

main: main.o server_class.o client_class.o util.o
	$(CCC) $(CCFLAGS) main.o server_class.o client_class.o util.o -o proxy
main.o: main.cc
	$(CCC) $(CCFLAGS) -c main.cc
server_class.o: server_class.cc
	$(CCC) $(CCFLAGS) -c server_class.cc
client_class.o: client_class.cc
	$(CCC) $(CCFLAGS) -c client_class.cc
util.o: util.cc
	$(CCC) $(CCFLAGS) -c util.cc

# Städa arbetsmappen
clean:
	@ \rm -f *.o
