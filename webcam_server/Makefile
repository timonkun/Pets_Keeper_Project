CC = gcc
CROSS_CC = /opt/toolchains/arm-2009q3/bin/arm-none-linux-gnueabi-gcc

arm : webcam_server.arm

webcam_server.arm : webcam_server.c
	$(CROSS_CC) $< -static -o $@

all : webcam_server

webcam_server : webcam_server.c
	$(CC) $< -o $@

clean :
	rm webcam_server webcam_server.arm
