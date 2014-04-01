controller.o: controller.c controller.h
data.o: data.c
main.o: main.c controller.h server.h
network.o: network.c network.h
ptc.o: ptc.c mcafx.h bool.h ../mcprotocal.h ptc.h types.h network.h \
 server.h
server.o: server.c mcafx.h bool.h ../mcprotocal.h server.h types.h \
 network.h controller.h ptc.h
stack.o: stack.c stack.h
