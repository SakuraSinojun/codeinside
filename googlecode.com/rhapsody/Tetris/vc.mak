


CC      = @cl
CXX     = @cl
LD      = @link

CFLAGS  = /nologo /c /O2 /W3 /WX
LFTRGT  = /nologo user32.lib gdi32.lib winmm.lib msimg32.lib

COUT	= /Fo
LOUT	= /OUT:

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
        
all: $(TETRIS)

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
	del *.o /Q
















