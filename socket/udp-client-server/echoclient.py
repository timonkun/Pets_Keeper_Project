#!/usr/bin/env python 
from socket import *

sock = socket(AF_INET, SOCK_DGRAM)
while True:
	data = raw_input('>')
	sock.sendto(data, ('pets-keeper.com', 9999))
	if not data: break;
	newdata = sock.recvfrom(1024)
	print newdata
sock.close
