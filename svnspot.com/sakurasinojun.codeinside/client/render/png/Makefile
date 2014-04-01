

CC     ?= gcc
CXX    ?= g++
LD     ?= g++

CFLAGS  = -c -O -Werror -Wall
LFRNDR  = -shared
INCLUDES = -I../../../common -I../zlib

COUT	= -o
LOUT	= -o

OUTPATH = .

TARGET = libpng.a
TEST   = test.exe

#$(wildcard *.o)


OBJS = png.o		\
       pngerror.o	\
       pngget.o		\
       pngmem.o		\
       pngpread.o	\
       pngread.o	\
       pngrio.o		\
       pngrtran.o	\
       pngrutil.o	\
       pngset.o		\
       pngtrans.o	\
       pngwio.o		\
       pngwrite.o	\
       pngwtran.o	\
       pngwutil.o


all: $(TARGET) 

$(TARGET): $(OBJS)
	@echo Archiving $(TARGET) ...
	@ar -csvr $(TARGET) $(OBJS)
	@echo ---------------------------------
	@echo done.

$(TEST): test.o $(OBJS)
	echo Compiling test....
	$(LD) test.o $(OBJS) -otest.exe

.cpp.o:
	@echo Compling $@ ...
	$(CXX) $< $(CFLAGS) $(COUT)$@
	@echo ---------------------------------
	
.c.o:
	@echo Compling $@ ...
	$(CC) $< $(CFLAGS) $(INCLUDES) $(COUT)$@ -D__TEST
	@echo ---------------------------------

.PHONY: all clean

clean:
	rm -fr *.o *.so *.a
















