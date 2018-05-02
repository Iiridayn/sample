# mud makefile

CC = gcc
FLAGS = -g
EXE_NAME = mud
OFILES = player.o

# make executable

$(EXE_NAME):	$(OFILES)
		$(CC) $(FLAGS) -o $(EXE_NAME) $(OFILES) -lpthreads
		chmod 711 $(EXE_NAME)

# make .o files

init.o:	globals.h init.c
		$(CC) $(FLAGS) -c init.c

player.o:	player.h globals.h player.c
		$(CC) $(FLAGS) -c player.c


clean:		
		rm -f *~
		rm -f *.o
		rm -f $(EXE_NAME)
