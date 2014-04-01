

CXX	:= g++
LD	:= g++
AR	:= ar
CXXFLAGS := -Wall -O2 -Werror
INCLUDES = -I../common -I../log

TARGET	= gate
LINKS	= -L.
LIBS	=

SOURCES := main.cc
SOURCES += player.cc
SOURCES += game.cc
SOURCES += poker.cc

SOURCES += ../log/iptv_logging.cc
SOURCES += ../log/log_tracer.cc
SOURCES += ../log/log_tracer_shell.cc
SOURCES += ../log/log_tracer_file.cc
SOURCES += ../log/log.cc

SOURCES += ../common/tools/socket.cc
SOURCES += ../common/tools/select.cc
SOURCES += ../common/packet/server_info_packet.cc
SOURCES += ../common/packet/hello_packet.cc
SOURCES += ../common/packet/deal_packet.cc
SOURCES += ../common/packet/message_packet.cc
SOURCES += ../common/packet/heartbit_packet.cc
SOURCES += ../common/packet/game_start_packet.cc
SOURCES += ../common/packet/scene_packet.cc
SOURCES += ../common/packet/play_packet.cc
SOURCES += ../common/Archive.cc
SOURCES += ../common/packet_parser.cc
SOURCES += ../common/util/string.cc
SOURCES += ../common/rules/gouji.cc


OBJS := $(SOURCES:.cc=.o)
DEPS := $(SOURCES:.cc=.d)

all: prebuild $(TARGET)

$(TARGET): $(OBJS)
	@echo Linking $@ ...
	$(LD) $(OBJS) $(LINKS) $(LIBS) -o$@
	@echo -------------------------------------------
	@echo done.

.cpp.o:
	@echo Compling $@ ...
	$(CXX) -c $< $(INCLUDES) $(CXXFLAGS)  -o $@
	@echo -------------------------------------------

.cc.o:
	@echo Compling $@ ...
	$(CXX) -c $< $(INCLUDES) $(CXXFLAGS)  -o $@
	@echo ------------------------------------------

%.d:%.cc
	$(CXX) -MM $< $(INCLUDES) $(CXXFLAGS) -o $@

prebuild:
	#echo -------

include $(DEPS)

clean:
	rm -fr $(OBJS) $(DEPS)



