

CC     ?= gcc
CXX    ?= g++
LD     ?= g++

CFLAGS  = -c -O -Werror -Wall
LFRNDR  = -shared
INCLUDES = -I../../../common

COUT	= -o
LOUT	= -o

OUTPATH = .

TARGET = libjpeg.a
TEST   = test.exe

#$(wildcard *.o)


OBJS = 	jaricom.o \
        jcomapi.o \
        jutils.o \
        jerror.o \
        jmemmgr.o \
        jmemnobs.o \
        jcapimin.o \
        jcapistd.o \
        jcarith.o \
        jctrans.o \
        jcparam.o \
        jdatadst.o \
        jcinit.o \
        jcmaster.o \
        jcmarker.o \
        jcmainct.o \
        jcprepct.o \
        jccoefct.o \
        jccolor.o \
        jcsample.o \
        jchuff.o \
        jcdctmgr.o \
        jfdctfst.o \
        jfdctflt.o \
        jfdctint.o \
        jdapimin.o \
        jdapistd.o \
        jdarith.o \
        jdtrans.o \
        jdatasrc.o \
        jdmaster.o \
        jdinput.o \
        jdmarker.o \
        jdhuff.o \
        jdmainct.o \
        jdcoefct.o \
        jdpostct.o \
        jddctmgr.o \
        jidctfst.o \
        jidctflt.o \
        jidctint.o \
        jdsample.o \
        jdcolor.o \
        jquant1.o \
        jquant2.o \
        jdmerge.o


all: $(TARGET) $(TEST)

$(TARGET): $(OBJS)
	@echo Archiving libjpeg.a ...
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
















