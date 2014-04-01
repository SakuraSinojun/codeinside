

CXX	?= i586-mingw32msvc-g++
LD	?= i586-mingw32msvc-g++
AR	?= i586-mingw32msvc-ar
CXXFLAGS := -Wall -O2 -Werror $(EXCXXFLAGS)
INCLUDES := 

TARGET	= libcommon.a

OBJS := tools/socket.o
OBJS += tools/select.o
OBJS += Archive.o	
OBJS += tools/dump.o
OBJS += tools/config.o
OBJS += tools/lock.o

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo Linking $@ ...
	$(AR) crs $@ $(OBJS)
	@echo -------------------------------------------
	@echo done.


.cpp.o:
	@echo Compling $@ ...
	$(CXX) -c $< $(CXXFLAGS) $(INCLUDES) -o $@
	@echo -------------------------------------------

.cc.o:
	@echo Compling $@ ...
	$(CXX) -c $< $(CXXFLAGS) $(INCLUDES) -o $@
	@echo -------------------------------------------


clean:
	rm -fr $(OBJS) $(TARGET)


