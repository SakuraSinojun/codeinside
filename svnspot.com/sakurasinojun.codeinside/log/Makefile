

CXX	?= i586-mingw32msvc-g++
LD	?= i586-mingw32msvc-g++
AR	?= i586-mingw32msvc-ar
CXXFLAGS := -Wall -O2 -Werror
INCLUDES = 

TARGET	= liblog.a
LINKS	= -L.
LIBS	=

OBJS	= iptv_logging.o log_tracer.o log_tracer_shell.o log.o

all: $(TARGET)


$(TARGET): $(OBJS)
	@echo Linking $@ ...
	$(AR) crs $@ $(OBJS)
	@echo -------------------------------------------
	@echo done.

.cpp.o:
	@echo Compling $@ ...
	$(CXX) -c $< $(CXXFLAGS) $(COUT) -o $@
	@echo -------------------------------------------

.cc.o:
	@echo Compling $@ ...
	$(CXX) -c $< $(CXXFLAGS) $(COUT) -o $@
	@echo ------------------------------------------

clean:
	rm -fr *.o *.a


