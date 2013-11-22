#!/usr/bin/env python

from socket import *

sock = socket(AF_INET, SOCK_DGRAM)
sock.bind(('0.0.0.0', 8000))

while True:
	data, address = sock.recvfrom(1024)
	if not data: break;
	sock.sendto(data, address)
sock.close()
