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
user_list = {}
local_ip = socket.gethostbyname(socket.gethostname())
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def server_handle(): 
    print'客户端线程已经启动 , 等待其它客户端连接'
    while True:
        data, addr = sock.recvfrom(8192) 
        data_str = data.split('#') 
        data_type = data_str[0] 
        data_info = data_str[1] 
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
        if data_type == 'keepconnect':
		    messeg = 'type:alive'
		    sock.sendto(messeg, addr)

###MAIN###
if __name__ == '__main__': 
    thread.start_new_thread(server_handle, ()) 
    time.sleep(0.1) 
    cmd = raw_input('输入指令>>') 
    while True: 
        args = cmd.split(' ') 
        if args[0] == 'login': 
            user_name = args[1] 
            local_uname = args[1] 
            address = "private_ip:%s private_port:%d" % (local_ip, CLIENT_PORT) 
            headder = "type:login\tuser_name:%s\tprivate_ip:%s\tprivate_port:%d" % (user_name,local_ip,CLIENT_PORT) 
            sock.sendto(headder, (SERVER_IP, SERVER_PORT)) 
        elif args[0] == 'getalluser': 
            headder = "type:getalluser\tuser_name:al"
            sock.sendto(headder,(SERVER_IP,SERVER_PORT)) 
            print'获取用户列表中。。。'
            time.sleep(1) 
            for one_user in user_list: 
                print'username:%s pub_ip:%s pub_port:%s pri_ip:%s pri_port:%s' % (one_user,user_list[one_user]['pub_ip'], \
                user_list[one_user]['pub_port'],user_list[one_user]['pri_ip'],user_list[one_user]['pri_port']) 
        elif args[0] == 'connect': 
            user_name = args[1] 
            to_user_ip = user_list[user_name]['pub_ip'] 
            to_user_port = int(user_list[user_name]['pub_port']) 
        elif args[0] =='echo': 
            m = ' '.join(args[1:]) 
            messeg = 'echo#from %s:%s'%(local_uname,m) 
            sock.sendto(messeg, (to_user_ip, to_user_port)) 
            time.sleep(0.1)  
        elif args[0] == 'move':
			m = ' '.join(args[1:])
			messeg = 'move#%s'%(m)
			sock.sendto(messeg, (to_user_ip, to_user_port))
			time.sleep(0.1)
        cmd = raw_input('输入指令>>') 
