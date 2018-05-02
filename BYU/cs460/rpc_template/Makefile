# CS 460
# Lab 4

# macro definitions
CC=g++ -g
C_FLAGS=
LINK_FLAGS=-lnsl -lpthread
# for visual C++ -ML
# -g +eh
EXE_NAME=rpc

# program executable *********
$(EXE_NAME): main.o protocol.o message.o rpc.o tcp.o arp.o ethernet.o internet.o physical.o
	$(CC) -v -o $(EXE_NAME) main.o rpc.o tcp.o arp.o protocol.o message.o ethernet.o internet.o physical.o $(LINK_FLAGS)
		
# .o files *****************

main.o:	main.cc protocol.h message.h
	$(CC) $(C_FlAGS) -c main.cc	

protocol.o: protocol.cc protocol.h message.h
	$(CC) $(C_FLAGS) -c protocol.cc	

physical.o: physical.cc physical.h message.h
	$(CC) $(C_FLAGS) -c physical.cc	

ethernet.o: ethernet.cc ethernet.h message.h
	$(CC) $(C_FLAGS) -c ethernet.cc	

arp.o: arp.cc arp.h message.h
	$(CC) $(C_FLAGS) -c arp.cc	

internet.o: internet.cc internet.h message.h
	$(CC) $(C_FLAGS) -c internet.cc	

tcp.o: tcp.cc tcp.h message.h
	$(CC) $(C_FLAGS) -c tcp.cc	

rpc.o: rpc.cc rpc.h message.h
	$(CC) $(C_FLAGS) -c rpc.cc	

message.o: message.cc message.h
	$(CC) $(C_FLAGS) -c message.cc

clean:
	rm *.o
