


CC      = @cl
CXX     = @cl
LD      = @link
LB      = @lib

CFLAGS  = /nologo /c /O2 /W3 /MT

COUT	= /Fo
LOUT	= /OUT:

#VPATH = .\jpeg6
OUTPATH = .

TARGET = $(OUTPATH)\libjpeg.lib
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


all: $(TARGET)

$(TARGET): $(OBJS)
	@echo Linking $@ ...
	$(LB) /nologo $(OBJS) $(LOUT)"$@"
	@echo ---------------------------------
	@echo done.

.cpp.o:
	@echo Compling $@ ...
	$(CXX) $< $(CFLAGS) $(COUT)"$@"
	@echo ---------------------------------
	
.c.o:
	@echo Compling $@ ...
	$(CC) $< $(CFLAGS) $(COUT)"$@"
	@echo ---------------------------------

.PHONY: all clean

clean:
	del *.o /Q
















