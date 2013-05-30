arm : webcam_server.arm

webcam_server.arm : webcam_server.c
	arm-none-linux-gnueabi-gcc webcam_server.c -static -o webcam_server.arm

all : webcam_server

webcam_server : webcam_server.c
	gcc webcam_server.c -o webcam_server

clean :
	rm webcam_server webcam_server.arm
