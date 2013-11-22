#!/usr/bin/env python

"Usage: echoserver.py <port>"

from socket import *
import sys

def HandleClient(connect, socket):
	while True:
		data = connect.recv(128)
		recv_len = len(data)
		if recv_len <= 0:
			print 'recv_len=', recv_len
			break
		print 'Received buffer:', data
		connect.send(data)

		if data == 'quit\r\n':
			break

	connect.close()

#MAIN
if __name__ == '__main__':
	
	if len(sys.argv) != 2:
		print __doc__
		sys.exit(0)

	sock = socket(AF_INET, SOCK_STREAM)
	host = '0.0.0.0'#gethostname()
	port = int(sys.argv[1])

	sock.bind((host, port))

	sock.listen(5)
	try:
		while True:
			connect, addr = sock.accept()
			print 'Got connection from ', addr
			connect.send('Thanks for connecting>>')
			HandleClient(connect, socket)
			#connect.close()
	except:
		sock.close()
		print 'EXIT'
		sys.exit(0)

