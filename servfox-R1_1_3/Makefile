##############################
# servfox Makefile
##############################

INSTALLROOT=$(PWD)

#CC=arm-linux-gcc
CC=arm-none-linux-gnueabi-gcc
INSTALL=install
BIN=/usr/local/bin


SERVFLAGS= -O2 -DLINUX $(WARNINGS) -I/home/timonkun/Workspace/Linux设备驱动光盘/sd-card/source/android-kernel-samsung-dev-full/include

SERVLIBS= -lpthread -static

#WARNINGS = -Wall \
#           -Wundef -Wpointer-arith -Wbad-function-cast \
#           -Wcast-align -Wwrite-strings -Wstrict-prototypes \
#           -Wmissing-prototypes -Wmissing-declarations \
#           -Wnested-externs -Winline -Wcast-qual -W \
#           -Wno-unused
#           -Wunused



OBJSERVER= server.o color.o utils.o tcputils.o v4l2uvc.o avilib.o

		
all:	 servfox 

clean:
	@echo "Cleaning up directory."
	rm -f *.a *.o servfox   core *~ log errlog


servfox: $(OBJSERVER)
	$(CC) $(SERVFLAGS) -o servfox $(OBJSERVER) $(SERVLIBS)
	

	
server.o:	server.c
		$(CC) $(SERVFLAGS) -c -o $@ $<
	
#spcav4l.o:	spcav4l.c spcav4l.h
#		$(CC) $(SERVFLAGS) -c -o $@ $<
		
utils.o:	utils.c utils.h
		$(CC) $(SERVFLAGS) -c -o $@ $<
				
tcputils.o:	tcputils.c tcputils.h
		$(CC) $(SERVFLAGS) -c -o $@ $<

color.o: color.c color.h
	$(CC) $(SERVFLAGS) -c -o $@ $<

v4l2uvc.o: v4l2uvc.c v4l2uvc.h
		$(CC) $(SERVFLAGS) -c -o $@ $<

avilib.o: avilib.c avilib.h
	$(CC) $(SERVFLAGS) -c -o $@ $<

		
install_servfox: spcafox
	$(INSTALL) -s -m 755 -g root -o root servfox $(BIN) 
	

	 
install: servfox 
	$(INSTALL) -s -m 755 -g root -o root servfox $(BIN) 
	
