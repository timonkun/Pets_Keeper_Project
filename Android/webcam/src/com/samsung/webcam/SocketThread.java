package com.samsung.webcam;

import java.net.DatagramPacket;   
import java.net.DatagramSocket;   
import java.net.InetAddress;   
import java.net.InetSocketAddress;   
import java.net.SocketAddress;

/**
 * 通过实现Runnable接口创建一个线程
 * @author timonkun
 */
public class SocketThread implements Runnable {
    private static final String robot_name = "robot";
    private DatagramSocket socketClient; 
    private SocketAddress socketTarget;
    private UserName one_user;
    
    
    SocketThread(DatagramSocket socketClientParam, SocketAddress socketTargetParam, UserName one_userParam)
    {
        this.socketClient = socketClientParam;
        this.socketTarget = socketTargetParam;
        this.one_user = one_userParam;
    }
    
    public void run() {
        byte[] buf = new byte[1024];   
        DatagramPacket packet = new DatagramPacket(buf, buf.length);   

        System.out.println("SocketThread running!");
        try { 
            
            while(true)
            {//To be done...
                socketClient.receive(packet);
                int packetLength = packet.getLength();
                if(packetLength == 10)
                    packetLength = 15;
                String receiveMessage = new String(packet.getData(), 0, packetLength);   
                System.out.println(receiveMessage + "  LEN=" + packet.getLength());   
    
                // +++ separate the words +++
                String[] sRecvArray = receiveMessage.split("#");
                String[] sRecvString = new String[2];
                int i=0;
                for(String str: sRecvArray)
                {
                    sRecvString[i] = str;
                    i++;
                }
                String data_type = sRecvString[0];
                String data_info = sRecvString[1];
               // --- separate the words ---
                
                System.out.println(data_type + "@");   
                if(data_type.equals("info"))
                {
                    System.out.println(data_info);
                }
                if(data_type.equals("getalluser"))
                {
                    /***
                     * print 'enter getalluser'
                        data_sp = data_info.split(' ')
                        user_name = data_sp[0].split(':')[1]
                        del data_sp[0]
                        user_list[user_name] = {}
                        for one_line in data_sp:
                            arg = one_line.split(':')
                            user_list[user_name][arg[0]] = arg[1]
                     */
                    /* getalluser#username:%s pub_ip:%s pub_port:%s pri_ip:%s pri_port:%s */
                    // +++ separate the words +++
                    String[] data_sp = data_info.split(" ");
                    String[] data_str = new String[5];
                    int j=0;
                    for(String str: data_sp)
                    {
                        data_str[j] = str;
                        j++;
                    }
                    // --- separate the words ---
                    
                    for(int k=0; k<5; k++)
                    {
                        String[] name_sp = data_str[k].split(":");
                        data_str[k] = name_sp[1];
                    }
                    // find the "robot" client
                    //if (data_str[0].equals(robot_name))
                    {
                        this.one_user.setAllParams(data_str[0], data_str[1],data_str[2],data_str[3],data_str[4]);
                        this.one_user.printUser();
                    }
                }
                if(data_type.equals("echo"))
                {
                    System.out.println("!!!echo!!!" + data_info);
                }
                if(data_type.equals("keepconnect"))
                {
                    System.out.println("!!!keepconnect!!!" + data_info);
                    String messeg = "type:alive";
                    byte[] sendbuf = messeg.getBytes();   
                    packet = new DatagramPacket(sendbuf, sendbuf.length, socketTarget);
                    socketClient.send(packet);  // type:alive
                }
            }
        } catch (Exception e) {   
            e.printStackTrace();   
        } 
    }
}

