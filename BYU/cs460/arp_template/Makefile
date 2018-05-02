# CS 460
# Lab 4

# macro definitions
CC=g++ -g
C_FLAGS=
LINK_FLAGS=-lnsl -lpthread
# for visual C++ -ML
# -g +eh
EXE_NAME=arp

# program executable *********
arp: main.o protocol.o message.o arp.o internet.o ethernet.o physical.o
	$(CC) -v -o $(EXE_NAME) main.o protocol.o message.o arp.o internet.o ethernet.o physical.o $(LINK_FLAGS)

client: main.o protocol.o message.o ethernet.o ip.o physical.o
	$(CC) -v -o client main.o protocol.o message.o ethernet.o ip.o physical.o $(LINK_FLAGS)
		
# .o files *****************

main.o:	main.cc protocol.h message.h physical.h ethernet.h internet.h
	$(CC) $(C_FlAGS) -c main.cc	

protocol.o: protocol.cc protocol.h message.h
	$(CC) $(C_FLAGS) -c protocol.cc	

physical.o: physical.cc physical.h message.h
	$(CC) $(C_FLAGS) -c physical.cc	

ethernet.o: ethernet.cc ethernet.h message.h
	$(CC) $(C_FLAGS) -c ethernet.cc	

internet.o: internet.cc internet.h message.h
	$(CC) $(C_FLAGS) -c internet.cc	

ip.o: ip.cc internet.h message.h
	$(CC) $(C_FLAGS) -c ip.cc	

message.o: message.cc message.h
	$(CC) $(C_FLAGS) -c message.cc

clean:
	rm *.o
