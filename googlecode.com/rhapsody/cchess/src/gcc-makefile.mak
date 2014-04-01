
CC = g++
LD = g++
CFLAGS = -c -Wall -O -Werror
LDFLAGS = C:\MinGW\lib\libgdi32.a C:\MinGW\lib\libwinmm.a C:\MinGW\lib\libuser32.a C:\MinGW\lib\libmsimg32.a -mwindows


OBJS = cchess.o \
	chessboard.o \
	piece.o \
	piecerook.o \
	pieceknight.o \
	piecebishop.o \
	piecemandarin.o \
	pieceking.o \
	piececannon.o \
	piecepawn.o \
	chinesechess.o \
	window.o \
	preload.o

TARGET = ../ÖÐ¹úÏóÆå.exe

all: $(TARGET) clean

$(TARGET): $(OBJS)
	@echo Linking $@ ...
	$(LD) $(OBJS) $(LDFLAGS) -o $@


.cpp.o:
	@echo Compling $< ...
	$(CC) $< $(CFLAGS) -o $@ 

rebuild: clean $(TARGET)


.PHONY: clean

clean:
	del *.o
	
	
	
	
	
	
	
	
	
	