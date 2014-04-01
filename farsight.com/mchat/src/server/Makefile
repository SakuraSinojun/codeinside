

CC	= cl
CXX	= cl
LD	= link
RC	= rc


COUT	= /Fo
LOUT	= /OUT:

OUTPATH	= ..\..\release\server

CFLAGS	= /c /O2 /nologo /W3 /WX /D_CRT_SECURE_NO_WARNINGS /Ipthread /EHsc
LDFLAGS	= /nologo /LIBPATH:pthread ws2_32.lib pthreadVC2.lib


TARGET	= $(OUTPATH)\server.exe

include objs.mak

all: $(TARGET) test.exe

$(TARGET): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) $(LOUT)"$@"

test.exe: test.o network.o
	$(LD) test.o network.o $(LDFLAGS) $(LOUT)"$(OUTPATH)\$@"

.cpp.o:
	$(CXX) $< $(CFLAGS) $(COUT)$@
	
.c.o:
	$(CC) $< $(CFLAGS) $(COUT)$@
	
%.RES: %.rc
	$(RC) $<

include d.mak

.PHONY: clean

clean:
	del *.o *.RES /Q $(OUTPATH)\test.exe *.ink

















