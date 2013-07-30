##############################
# servfox Makefile
##############################

INSTALLROOT=$(PWD)

#CC=arm-linux-gcc
CC=/opt/toolchains/arm-2009q3/bin/arm-none-linux-gnueabi-gcc
INSTALL=install
BIN=/usr/local/bin


SERVFLAGS= -O2 -DLINUX $(WARNINGS) -I/home/timonkun/Workspace/Linux设备驱动光盘/sd-card/source/android-kernel-samsung-dev-full/include

SERVLIBS= -lpthread

#WARNINGS = -Wall \
#           -Wundef -Wpointer-arith -Wbad-function-cast \
#           -Wcast-align -Wwrite-strings -Wstrict-prototypes \
#           -Wmissing-prototypes -Wmissing-declarations \
#           -Wnested-externs -Winline -Wcast-qual -W \
#           -Wno-unused
#           -Wunused



OBJSERVER= server.o spcav4l.o utils.o tcputils.o 

		
all:	 servfox 

clean:
	@echo "Cleaning up directory."
	rm -f *.a *.o servfox   core *~ log errlog


servfox: $(OBJSERVER)
	$(CC) $(SERVFLAGS) -o servfox $(OBJSERVER) $(SERVLIBS)
	

	
server.o:	server.c
		$(CC) $(SERVFLAGS) -c -o $@ $<
	
spcav4l.o:	spcav4l.c spcav4l.h
		$(CC) $(SERVFLAGS) -c -o $@ $<
		
utils.o:	utils.c utils.h
		$(CC) $(SERVFLAGS) -c -o $@ $<
				
tcputils.o:	tcputils.c tcputils.h
		$(CC) $(SERVFLAGS) -c -o $@ $<
		

		
install_servfox: spcafox
	$(INSTALL) -s -m 755 -g root -o root servfox $(BIN) 
	

	 
install: servfox 
	$(INSTALL) -s -m 755 -g root -o root servfox $(BIN) 
	
