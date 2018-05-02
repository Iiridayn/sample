# chess.cpp makefile

CC = g++
FLAGS = -g
EXE_NAME = chess
OFILES = chessgui.o chess.o messagebox.o buttonbox.o inputbox.o Board.o King.o Queen.o Rook.o Knight.o Bishop.o Pawn.o Piece.o

# make executable

$(EXE_NAME):	lib $(OFILES)
		$(CC) $(FLAGS) -o $(EXE_NAME) $(OFILES) `gtkmm-config --cflags --libs` -L. -lCS240utils
		chmod 711 $(EXE_NAME)
lib:		ObjectCountBase.o
		$(CC) -shared -o libCS240utils.so ObjectCountBase.o
# make .o files

ObjectCountBase.o:	ObjectCountBase.h ObjectCount.h
		$(CC) $(FLAGS) -c ObjectCountBase.cpp `gtkmm-config --cflags`

chessgui.o:	chessgui.h chessgui.cpp
		$(CC) $(FLAGS) -c chessgui.cpp `gtkmm-config --cflags`

chess.o:	chess.h chess.cpp Board.h
		$(CC) $(FLAGS) -c chess.cpp `gtkmm-config --cflags`

messagebox.o:	messagebox.h messagebox.cpp
		$(CC) $(FLAGS) -c messagebox.cpp `gtkmm-config --cflags`

buttonbox.o:	buttonbox.h buttonbox.cpp
		$(CC) $(FLAGS) -c buttonbox.cpp `gtkmm-config --cflags`

inputbox.o:	inputbox.h inputbox.cpp
		$(CC) $(FLAGS) -c inputbox.cpp `gtkmm-config --cflags`

Board.o:	Board.h Board.cpp Piece.h King.h
		$(CC) $(FLAGS) -c Board.cpp `gtkmm-config --cflags`

King.o:		King.h Piece.h King.cpp
		$(CC) $(FLAGS) -c King.cpp `gtkmm-config --cflags`

Queen.o:	Queen.h Piece.h Queen.cpp
		$(CC) $(FLAGS) -c Queen.cpp `gtkmm-config --cflags`

Rook.o:		Rook.h Piece.h Rook.cpp
		$(CC) $(FLAGS) -c Rook.cpp `gtkmm-config --cflags`

Knight.o:	Knight.h Piece.h Knight.cpp
		$(CC) $(FLAGS) -c Knight.cpp `gtkmm-config --cflags`

Bishop.o:	Bishop.h Piece.h Bishop.cpp
		$(CC) $(FLAGS) -c Bishop.cpp `gtkmm-config --cflags`

Pawn.o:		Pawn.h Piece.h Pawn.cpp
		$(CC) $(FLAGS) -c Pawn.cpp `gtkmm-config --cflags`

Piece.o:	Piece.h Piece.cpp
		$(CC) $(FLAGS) -c Piece.cpp `gtkmm-config --cflags`

clean:		
		rm -f *~
		rm -f *.o
		rm -f chess