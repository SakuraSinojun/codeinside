

CXX	?= i586-mingw32msvc-g++
LD	?= i586-mingw32msvc-g++
AR	?= i586-mingw32msvc-ar
CXXFLAGS ?= -Wall -O2 -Werror
INCLUDES ?= -I../../common -Ijpeg -Ipng

TARGET	= librender.a

OBJS	= Render.o	\
	  Animation.o	\
	  Surface.o

all: $(TARGET)

$(TARGET): $(OBJS) jpeg/*.o png/*.o zlib/*.o
	@echo Linking $@ ...
	$(AR) crs $@ $(OBJS) jpeg/*.o png/*.o zlib/*.o
	@echo -------------------------------------------
	@echo done.

.cpp.o:
	@echo Compling $@ ...
	$(CXX) -c $< $(CXXFLAGS) $(INCLUDES) -o $@
	@echo -------------------------------------------

jpeg/*.o:
	make -C jpeg

png/*.o:
	make -C png

zlib/*.o:
	make -C zlib

clean:
	rm -fr *.o *.a
	make -C jpeg clean
	make -C png clean
	make -C zlib clean



