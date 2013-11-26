#!/usr/bin/env python
# -*- coding: utf-8 -*-
"USAGE: p2p-client.py <server> <port>"

import sys, socket, SocketServer, threading, thread, time

if len(sys.argv) != 3:
	print __doc__
	sys.exit(0)

CLIENT_PORT = 4321
SERVER_IP = sys.argv[1] #"pets-keeper.com"
SERVER_PORT = int(sys.argv[2])#9998
CLIENT_NAME = 'mobile'
ROBOT_NAME = 'robot'
user_list = {}
local_ip = socket.gethostbyname(socket.gethostname())
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def move(direction):
	print direction

def server_handle(): 
    print'HOME ROBOT client start...'
    while True:
        data, addr = sock.recvfrom(8192) 
        data_str = data.split('#') 
        data_type = data_str[0] 
        data_info = data_str[1] 
        #print data_type, data_info
        if data_type == 'info':
            #print 'enter info'
            del data_str[0] 
            print data_info
        if data_type == 'getalluser':
            print 'enter getalluser'
            data_sp = data_info.split(' ')
            user_name = data_sp[0].split(':')[1]
            del data_sp[0]
            user_list[user_name] = {}
            for one_line in data_sp:
                arg = one_line.split(':')
                user_list[user_name][arg[0]] = arg[1]
        if data_type == 'echo' :
            print data_info
        if data_type == 'move':
            move(data_info)
        if data_type == 'keepconnect':
		    messeg = 'type:alive'
		    sock.sendto(messeg, addr)

###MAIN###
if __name__ == '__main__': 
    thread.start_new_thread(server_handle, ()) 
    time.sleep(0.1) 
    #cmd = raw_input('输入指令>>') 
    cmd = 'login' 
    if cmd == 'login': 
        user_name = ROBOT_NAME
        local_uname = ROBOT_NAME 
        address = "private_ip:%s private_port:%d" % (local_ip, CLIENT_PORT) 
        headder = "type:login\tuser_name:%s\tprivate_ip:%s\tprivate_port:%d" % \
				(user_name,local_ip,CLIENT_PORT) 
        sock.sendto(headder, (SERVER_IP, SERVER_PORT)) 
    
	cmd = 'getalluser'
    while True: 
		if cmd == 'getalluser': 
			headder = "type:getalluser\tuser_name:al"
			sock.sendto(headder,(SERVER_IP,SERVER_PORT)) 
			print'获取用户列表中。。。'
            #time.sleep(1) 
			for one_user in user_list:
				print'username:%s pub_ip:%s pub_port:%s pri_ip:%s pri_port:%s' % \
					(one_user,user_list[one_user]['pub_ip'], user_list[one_user]['pub_port'], \
					user_list[one_user]['pri_ip'],user_list[one_user]['pri_port'])
				if one_user == CLIENT_NAME:
					cmd = 'connect'
		elif cmd == 'connect': 
			user_name = CLIENT_NAME 
			to_user_ip = user_list[user_name]['pub_ip'] 
			to_user_port = int(user_list[user_name]['pub_port'])
			cmd = 'echo'
		elif cmd =='echo': 
			m = 'hello world!'#CLIENT_NAME 
			messeg = 'echo#from %s:%s'%(local_uname,m) 
			sock.sendto(messeg, (to_user_ip, to_user_port)) 
			time.sleep(0.1)  
		time.sleep(3)
		#cmd = raw_input('输入指令>>') 
