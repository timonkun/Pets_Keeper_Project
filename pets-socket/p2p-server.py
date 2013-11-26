#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket, sys, SocketServer, threading, thread, time
SERVER_PORT = 9998
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) 
sock.bind(('', SERVER_PORT)) 
user_list = [] 

def server_handle(): 
    while True: 
        cli_date, cli_pub_add = sock.recvfrom(8192) 
        now_user = [] 
        headder = [] 
        cli_str = {} 
        headder = cli_date.split('\t') 
        for one_line in headder: 
            str = {} 
            str = one_line 
            args = str.split(':') 
            cli_str[args[0]] = args[1] 
            
        if cli_str['type'] == 'login' : 
            del cli_str['type'] 
            now_user = cli_str 
            now_user['cli_pub_ip'] = cli_pub_add[0] 
            now_user['cli_pub_port'] = cli_pub_add[1] 
            user_list.append(now_user) 
            toclient = 'info#%s login in successful , the info from server' % now_user['user_name'] 
            sock.sendto(toclient,cli_pub_add)
            print'-'*100
            print "%s 已经登录,公网IP:%s 端口:%d\n" % (now_user['user_name'],now_user['cli_pub_ip'],now_user['cli_pub_port'])
            print "以下是已经登录的用户列表"
            print len(user_list)
            for one_user in user_list: 
                print'用户名:%s 公网ip:%s 公网端口:%s 私网ip:%s 私网端口:%s' % \
                (one_user['user_name'],one_user['cli_pub_ip'],one_user['cli_pub_port'],one_user['private_ip'],one_user['private_port'])
                
        elif cli_str['type'] == 'alive': 
            pass
        elif cli_str['type'] == 'logout' : 
            pass
        elif cli_str['type'] == 'getalluser' : 
            print '-'*100
            for one_user in user_list : 
                toclient = 'getalluser#username:%s pub_ip:%s pub_port:%s pri_ip:%s pri_port:%s'%(one_user['user_name'],one_user['cli_pub_ip'],one_user['cli_pub_port'],one_user['private_ip'],one_user['private_port']) 
                sock.sendto(toclient,cli_pub_add) 

if __name__ == '__main__': 
    thread.start_new_thread(server_handle, ()) 
    print'服务器进程已启动，等待客户连接'
    while True: 
        for one_user in user_list: 
            toclient = 'keepconnect#111'
            sock.sendto(toclient,(one_user['cli_pub_ip'],one_user['cli_pub_port']))  
            time.sleep(1) 
