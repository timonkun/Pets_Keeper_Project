##############################
# spcaview Makefile
##############################

INSTALLROOT=$(PWD)

CC=/opt/toolchains/arm-2009q3/bin/arm-none-linux-gnueabi-gcc
CPP=/opt/toolchains/arm-2009q3/bin/arm-none-linux-gnueabi-g++
INSTALL=install
APP_BINARY=luvcview
BIN=/home/timonkun/Workspace/webcam/luvcview-20070512-arm
#SDLLIBS = $(shell sdl-config --libs) 
#SDLFLAGS = $(shell sdl-config --cflags)
SDLLIBS = -L/home/timonkun/Workspace/webcam/SDL-ARM-linux-gcc4.4.1/lib -lSDL
SDLFLAGS = /home/timonkun/Workspace/webcam/SDL-ARM-linux-gcc4.4.1/include

#LIBX11FLAGS= -I/usr/X11R6/include -L/usr/X11R6/lib
VERSION = 0.2.1

#WARNINGS = -Wall \
#           -Wundef -Wpointer-arith -Wbad-function-cast \
#           -Wcast-align -Wwrite-strings -Wstrict-prototypes \
#           -Wmissing-prototypes -Wmissing-declarations \
#           -Wnested-externs -Winline -Wcast-qual -W \
#           -Wno-unused
#           -Wunused

CFLAGS += -DUSE_SDL -O2 -DLINUX -DVERSION=\"$(VERSION)\" -I$(SDLFLAGS) $(WARNINGS) -lpthread
CPPFLAGS = $(CFLAGS)

OBJECTS= luvcview.o color.o utils.o v4l2uvc.o gui.o avilib.o
		

all:	luvcview

clean:
	@echo "Cleaning up directory."
	rm -f *.a *.o $(APP_BINARY) core *~ log errlog *.avi

# Applications:
luvcview:	$(OBJECTS)
	$(CC)	$(CFLAGS) $(OBJECTS) $(X11_LIB) $(XPM_LIB)\
		$(MATH_LIB) \
		$(SDLLIBS)\
		-o $(APP_BINARY)
	chmod 755 $(APP_BINARY)


install: luvcview
	$(INSTALL) -s -m 755 -g root -o root $(APP_BINARY) $(BIN) 
	rm -f $(BIN)/uvcview
