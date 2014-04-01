

CC     ?= gcc
CXX    ?= g++
LD     ?= g++

CFLAGS  = -c -O -Werror -Wall -DNO_VIZ
LFRNDR  = -shared
INCLUDES = -I../../../common

COUT	= -o
LOUT	= -o

OUTPATH = .

TARGET = libz.a
TEST   = test.exe

#$(wildcard *.o)


OBJS =	adler32.o	\
	compress.o	\
	crc32.o		\
	deflate.o	\
	gzclose.o	\
	gzlib.o		\
	gzread.o	\
	gzwrite.o	\
	infback.o	\
	inffast.o	\
	inflate.o	\
	inftrees.o	\
	trees.o		\
	uncompr.o	\
	zutil.o

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
















