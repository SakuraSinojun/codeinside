


CC      = @gcc
CXX     = @g++
LD      = @g++

CFLAGS  = -c -O2 -Werror -Wall
LFTRGT  = -luser32 -lgdi32 -lwinmm -lmsimg32

COUT	= -o
LOUT	= -o

VPATH = .\
OUTPATH = .

TARGET = Tetris.exe
TETRIS = TetrisWin.exe


#$(wildcard *.o)


OBJS = 	Tetris.o \
        main.o \
        debug.o

OBJT =  tetris.o \
        win32.o \
        debug.o
        
all: $(TARGET) $(TETRIS)

$(TARGET): $(OBJS)
	@echo Linking $@ ...
	$(LD) $(OBJS) $(LFTRGT) $(LOUT)$@
	@echo ---------------------------------
	@echo done.

$(TETRIS): $(OBJT)
	@echo Linking $@ ...
	$(LD) $(OBJT) $(LFTRGT) $(LOUT)$@
	@echo ---------------------------------
	@echo done.


.cpp.o:
	@echo Compling $@ ...
	$(CXX) $< $(CFLAGS) $(COUT)$@
	@echo ---------------------------------
	
.c.o:
	@echo Compling $@ ...
	$(CC) $< $(CFLAGS) $(COUT)$@
	@echo ---------------------------------

.PHONY: all clean

clean:
	$(RM) *.o 
















