

CC = g++
LD = g++

CFLAGS = -c -O -Werror -Wall
LDFLAGS = -LC:\MinGW\lib -lgdi32 -luser32 -lmsimg32 -lwinmm -mwindows
CCOUT = -o 
LDOUT = -o 



OBJS = firework.o \
	window.o \
	fire.o
	
TARGET = firework.exe

all: $(TARGET) 

$(TARGET): $(OBJS)
	@echo Linking $@ ...
	$(LD) $(OBJS) $(LDFLAGS) $(LDOUT)$@


.cpp.o:
	@echo Compling $< ...
	$(CC) $< $(CFLAGS) $(CCOUT)$@ 

rebuild: clean $(TARGET)


.PHONY: clean

clean:
	del *.o


