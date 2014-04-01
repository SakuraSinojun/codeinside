

CXX	:= i586-mingw32msvc-g++
LD	:= i586-mingw32msvc-g++
AR	:= i586-mingw32msvc-ar
CXXFLAGS := -Wall -O2 -Werror
INCLUDES = 

TARGET	= libwindow.a
TEST	= test.exe
LINKS	= -L.
LIBS	= -lwindow -lgdi32 -luser32 -lws2_32 -lcomctl32

OBJS	=	App.o		\
		Button.o	\
		Edit.o		\
		Window.o	\
		WinMain.o	\
		WndProc.o	\
		View.o		\
		ComboBox.o	\
		Static.o

all: $(TARGET) 

$(TEST): $(TARGET) test.o
	@echo Linking $@ ...
	$(LD) $(LINKS) test.o $(LIBS) -o$@
	@echo -------------------------------------------
	@echo done.

$(TARGET): $(OBJS)
	@echo Linking $@ ...
	$(AR) crs $@ $(OBJS)
	@echo -------------------------------------------
	@echo done.

.cpp.o:
	@echo Compling $@ ...
	$(CXX) -c $< $(CXXFLAGS) $(COUT) -o $@
	@echo -------------------------------------------


clean:
	rm -fr *.o *.a


