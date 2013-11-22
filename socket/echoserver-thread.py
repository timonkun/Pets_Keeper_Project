#!/usr/bin/env python

"Usage: echoserver.py <port>"

from socket import *
import sys, syslog
import thread

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

	count = 0
	sock = socket(AF_INET, SOCK_STREAM)
	host = '0.0.0.0'#gethostname()
	port = int(sys.argv[1])

	sock.bind((host, port))

	sock.listen(5)
	try:
		while True:
			connect, addr = sock.accept()
			count = count + 1
			
			print '%d: Got connection from %s' % (count, addr)
			syslog.syslog('Incoming connection from')

			#connect.send('Thanks for connecting>>')
			thread.start_new_thread(HandleClient, (connect,socket))
			#HandleClient(connect, socket)
			#connect.close()
	except KeyboardInterrupt:
		sock.close()
		print 'EXIT'
		sys.exit(0)

