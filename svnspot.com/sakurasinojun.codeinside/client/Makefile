

CXX	:= i586-mingw32msvc-g++
CC	:= i586-mingw32msvc-gcc
LD	:= i586-mingw32msvc-g++
AR	:= i586-mingw32msvc-ar
CXXFLAGS := -Wall -O2 -Werror
CFLAGS := -Wall -O2 -Werror -Wno-strict-aliasing
INCLUDES = -I../common -I../log -Irender/zlib

TARGET	= poker
LINKS	= -L.
LIBS	= -lws2_32 -lgdi32 -lcomctl32

SOURCES := main.cc
SOURCES += getopt.cc
SOURCES += connect.cc
SOURCES += game.cc
SOURCES += game_window.cc
SOURCES += packet_dispatcher.cc
SOURCES += info.cc

SOURCES += ../log/iptv_logging.cc
SOURCES += ../log/log_tracer.cc
SOURCES += ../log/log_tracer_shell.cc
SOURCES += ../log/log_tracer_file.cc
SOURCES += ../log/log.cc

SOURCES += ../common/Archive.cc
SOURCES += ../common/packet_parser.cc
SOURCES += ../common/tools/socket.cc
SOURCES += ../common/tools/lock.cc
SOURCES += ../common/packet/hello_packet.cc
SOURCES += ../common/packet/deal_packet.cc
SOURCES += ../common/packet/server_info_packet.cc
SOURCES += ../common/packet/message_packet.cc
SOURCES += ../common/packet/heartbit_packet.cc
SOURCES += ../common/packet/game_start_packet.cc
SOURCES += ../common/packet/scene_packet.cc
SOURCES += ../common/packet/play_packet.cc
SOURCES += ../common/rules/gouji.cc

SOURCES += window/App.cc
SOURCES += window/Window.cc
SOURCES += window/View.cc
SOURCES += window/WndProc.cc

SOURCES += render/Surface.cc
SOURCES += render/Animation.cc
SOURCES += render/Render.cc

SOURCES += render/jpeg/jaricom.c
SOURCES += render/jpeg/jcomapi.c
SOURCES += render/jpeg/jutils.c
SOURCES += render/jpeg/jerror.c
SOURCES += render/jpeg/jmemmgr.c
SOURCES += render/jpeg/jmemnobs.c
SOURCES += render/jpeg/jcapimin.c
SOURCES += render/jpeg/jcapistd.c
SOURCES += render/jpeg/jcarith.c
SOURCES += render/jpeg/jctrans.c
SOURCES += render/jpeg/jcparam.c
SOURCES += render/jpeg/jdatadst.c
SOURCES += render/jpeg/jcinit.c
SOURCES += render/jpeg/jcmaster.c
SOURCES += render/jpeg/jcmarker.c
SOURCES += render/jpeg/jcmainct.c
SOURCES += render/jpeg/jcprepct.c
SOURCES += render/jpeg/jccoefct.c
SOURCES += render/jpeg/jccolor.c
SOURCES += render/jpeg/jcsample.c
SOURCES += render/jpeg/jchuff.c
SOURCES += render/jpeg/jcdctmgr.c
SOURCES += render/jpeg/jfdctfst.c
SOURCES += render/jpeg/jfdctflt.c
SOURCES += render/jpeg/jfdctint.c
SOURCES += render/jpeg/jdapimin.c
SOURCES += render/jpeg/jdapistd.c
SOURCES += render/jpeg/jdarith.c
SOURCES += render/jpeg/jdtrans.c
SOURCES += render/jpeg/jdatasrc.c
SOURCES += render/jpeg/jdmaster.c
SOURCES += render/jpeg/jdinput.c
SOURCES += render/jpeg/jdmarker.c
SOURCES += render/jpeg/jdhuff.c
SOURCES += render/jpeg/jdmainct.c
SOURCES += render/jpeg/jdcoefct.c
SOURCES += render/jpeg/jdpostct.c
SOURCES += render/jpeg/jddctmgr.c
SOURCES += render/jpeg/jidctfst.c
SOURCES += render/jpeg/jidctflt.c
SOURCES += render/jpeg/jidctint.c
SOURCES += render/jpeg/jdsample.c
SOURCES += render/jpeg/jdcolor.c
SOURCES += render/jpeg/jquant1.c
SOURCES += render/jpeg/jquant2.c
SOURCES += render/jpeg/jdmerge.c

SOURCES += render/zlib/adler32.c
SOURCES += render/zlib/compress.c
SOURCES += render/zlib/crc32.c
SOURCES += render/zlib/deflate.c
SOURCES += render/zlib/gzclose.c
SOURCES += render/zlib/gzlib.c
SOURCES += render/zlib/gzread.c
SOURCES += render/zlib/gzwrite.c
SOURCES += render/zlib/infback.c
SOURCES += render/zlib/inffast.c
SOURCES += render/zlib/inflate.c
SOURCES += render/zlib/inftrees.c
SOURCES += render/zlib/trees.c
SOURCES += render/zlib/uncompr.c
SOURCES += render/zlib/zutil.c

SOURCES += render/png/png.c
SOURCES += render/png/pngerror.c
SOURCES += render/png/pngget.c
SOURCES += render/png/pngmem.c
SOURCES += render/png/pngpread.c
SOURCES += render/png/pngread.c
SOURCES += render/png/pngrio.c
SOURCES += render/png/pngrtran.c
SOURCES += render/png/pngrutil.c
SOURCES += render/png/pngset.c
SOURCES += render/png/pngtrans.c
SOURCES += render/png/pngwio.c
SOURCES += render/png/pngwrite.c
SOURCES += render/png/pngwtran.c
SOURCES += render/png/pngwutil.c


OBJS := $(SOURCES:.cc=.obj)
DEPS := $(SOURCES:.cc=.d)
OBJS := $(OBJS:.c=.obj)
DEPS := $(DEPS:.c=.d)

all: prebuild $(TARGET)

$(TARGET): $(OBJS)
	@echo Linking $@ ...
	$(LD) $(OBJS) $(LINKS) $(LIBS) -o$@
	@echo -------------------------------------------
	@echo done.

.cpp.obj:
	@echo Compling $@ ...
	$(CXX) -c $< $(INCLUDES) $(CXXFLAGS) -o $@
	@echo -------------------------------------------

.cc.obj:
	@echo Compling $@ ...
	$(CXX) -c $< $(INCLUDES) $(CXXFLAGS) -o $@
	@echo ------------------------------------------

.c.obj:
	@echo Compling $@ ...
	$(CC) -c $< $(INCLUDES) $(CFLAGS) -o $@
	@echo ------------------------------------------

%.obj:%.cc
	@echo Compling $@ ...
	$(CXX) -c $< $(INCLUDES) $(CXXFLAGS) -o $@
	@echo ------------------------------------------

%.obj:%.cpp
	@echo Compling $@ ...
	$(CXX) -c $< $(INCLUDES) $(CXXFLAGS) -o $@
	@echo ------------------------------------------

%.obj:%.c
	@echo Compling $@ ...
	$(CC) -c $< $(INCLUDES) $(CFLAGS) -o $@
	@echo ------------------------------------------

%.d:%.cc
	$(CXX) -MM $< $(INCLUDES) $(CXXFLAGS) -o $@

%.d:%.cpp
	$(CXX) -MM $< $(INCLUDES) $(CXXFLAGS) -o $@

%.d:%.c
	$(CC) -MM $< $(INCLUDES) $(CFLAGS) -o $@

prebuild:
	#make -C ../common clean

include $(DEPS)

clean:
	rm -fr $(OBJS) $(DEPS)



