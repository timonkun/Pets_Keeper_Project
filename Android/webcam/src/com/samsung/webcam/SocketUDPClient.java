package com.samsung.webcam;

import java.net.DatagramPacket;   
import java.net.DatagramSocket;   
import java.net.InetAddress;   
import java.net.InetSocketAddress;   
import java.net.SocketAddress;

public class SocketUDPClient {
    
    private static final int    CLIENT_PORT = 4321;
    private static final String SERVER_IP = "pets-keeper.com";
    private static final int    SERVER_PORT = 9998;
    private static final String mobile_name = "mobile";
    public UserName one_user;  
    private SocketAddress socketTarget;
    public DatagramSocket socketClient; 
    
    public SocketUDPClient()
    {
        try {
            socketTarget = new InetSocketAddress(SERVER_IP, SERVER_PORT);
            socketClient = new DatagramSocket();
            one_user = new UserName();
        } catch (Exception e) {
            e.printStackTrace();   
        }
    }
    
    public void setupSocket() 
    {
        SocketThread tt = new SocketThread(socketClient, socketTarget, one_user);
        Thread t = new Thread(tt);
        t.start();
        
        try {
            //InetAddress local_addr = null;
            
            String local_ip = InetAddress.getLocalHost().getHostAddress();
                    
            String headder = "type:login\tuser_name:"+mobile_name + "\tprivate_ip:" + local_ip + "\tprivate_port:"+ CLIENT_PORT;
            System.out.println(headder);
            byte[] sendbuf = headder.getBytes();   

            DatagramPacket pack = new DatagramPacket(sendbuf, sendbuf.length, socketTarget);   
            socketClient.send(pack);  // login mobile
            
            headder = "type:getalluser\tuser_name:al";
            sendbuf = headder.getBytes();   
            pack = new DatagramPacket(sendbuf, sendbuf.length, socketTarget);
            socketClient.send(pack);  // getalluser
                    
        } catch (Exception e) {
            e.printStackTrace();   
        }
    }
    
    public void registerClient()
    {
        System.out.println("registerClient() one_user.isNull()=" + one_user.isNull());
        if(one_user.isNull())
        {
            return;
        }
        
        
        String to_user_ip = one_user.getCliPubIP();
        int to_user_port = one_user.getCliPubPort();
        SocketAddress socketRobot = new InetSocketAddress(to_user_ip, to_user_port);
        
        // send echo
        String headder = "echo#from "+ mobile_name + " echo";
        byte[] sendbuf = headder.getBytes();   
        try {
            DatagramPacket pack = new DatagramPacket(sendbuf, sendbuf.length, socketRobot);
            socketClient.send(pack);  // echo
        } catch (Exception e) {
            e.printStackTrace();   
        }
    }

}
