#!/usr/bin/env python

"Usage: echoserver.py <port>"

from socket import *
import sys, os, syslog, signal

###GLOBAL VARIABAL###
children_list = []

###FUNCTIONS###
def HandleClient(connect):
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

def fireman(pids):
	while children_list:
		pid, status = os.waitpid(0, os.WNOHANG)
		if not pid: break;
		pids.remove(pid)

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
			print '%d Got connection from %s' %( count, addr)
			#connect.send('Thanks for connecting>>')
			fireman(children_list)
			pid = os.fork()

			if pid: #parent
				connect.close()
				children_list.append(pid)
			if not pid: #child pid==0
				sock.close()
				HandleClient(connect)
				os._exit(0)
	except KeyboardInterrupt:
		sock.close()
		print 'EXIT'
		sys.exit(0)

