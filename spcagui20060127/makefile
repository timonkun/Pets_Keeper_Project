# Simple makefile for SpcaGui

SDLLIBS =-lSDL_image $(shell sdl-config --libs)
SDLFLAGS = $(shell sdl-config --cflags)
LIBS = -ljpeg -lm
CFLAGS = -DUSE_SDL -O2 -DLINUX $(SDLFLAGS) -DHAVE_LIBJPEG=1
INSTALL=install
BIN=/usr/local/bin
INSTALL_DATA=${INSTALL} -m 644
INSTALL_DIR=mkdir -p
FONTDIR=icons/Fonts
ICONDIR=icons/Danil

OBJECTS= spcagui.o gui.o spcav4l.o spcadecoder.o utils.o avilib.o SFont.o intro.o

all: spcagui

spcagui: $(OBJECTS)  
	gcc $(CFLAGS) -o spcagui $(OBJECTS) $(SDLLIBS) $(LIBS)


install: spcagui
	$(INSTALL) -s -m 755 -g root -o root spcagui $(BIN)
	$(INSTALL_DIR) /usr/local/share/spcagui
	$(INSTALL_DATA) $(FONTDIR)/*.png /usr/local/share/spcagui
	$(INSTALL_DATA) $(ICONDIR)/*.png /usr/local/share/spcagui
clean:
	/bin/rm -f *.o spcagui
