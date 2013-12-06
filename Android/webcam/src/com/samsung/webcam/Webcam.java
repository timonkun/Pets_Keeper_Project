package com.samsung.webcam;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPReply;



/***
 * 
 * @author lingkun
 * 1. �������Զ����������ؽű������ݽű����ó�ʼ�����ذ�ť
    1.1 download
    1.2 read config
    1.3 initialize button
    1.4 �ʵ����ӳٷ����û�

   2. �û��������غ󣬸������ýű����ϴ�����
    2.1 write config
    2.2 upload 
 ***/

public class Webcam {
    private static final int OFF = 0;
    private static final int ON  = 1;
    
    private int g_webcam_ctrl = OFF;
    private int g_fan_ctrl = OFF;
    private int g_feed_ctrl = OFF;

    private static final String g_webcam_ctrl_str = "<CTRL_MODE_WEBCAM>";
    private static final String g_fan_ctrl_str = "<CTRL_MODE_FAN>";
    private static final String g_feed_ctrl_str = "<CTRL_MODE_FEED>";
    
    private static final String gFileName="control.txt";
    private static final String gDirName = "/storage/sdcard0/Download";
    private static final String gPathName=gDirName+gFileName;//�ļ��洢·�� 
    private static final String gUrlStr="http://timonkun.me/webcam/control.txt";  
    
    
    
    public Webcam(){
        
    } 
    
    public boolean getWebcamCtrl()
    {
        if(g_webcam_ctrl == ON)
            return true;
        else if(g_webcam_ctrl == OFF)
            return false;
        else 
            return false;
    }
    
    public void setWebcamCtrl(int value)
    {
        if(value == ON || value == OFF)
            g_webcam_ctrl = value;
        else
            return;
    }
    
    public boolean getFanCtrl()
    {
        if(g_fan_ctrl == ON)
            return true;
        else if(g_fan_ctrl == OFF)
            return false;
        else 
            return false;
    }
    
    public void setFanCtrl(int value)
    {
        if(value == ON || value == OFF)
            g_fan_ctrl = value;
        else
            return;
    }
    
    public boolean getFeedCtrl()
    {
        if(g_feed_ctrl == ON)
            return true;
        else if(g_feed_ctrl == OFF)
            return false;
        else 
            return false;
    }
    
    public void setFeedCtrl(int value)
    {
        if(value == ON || value == OFF)
            g_feed_ctrl = value;
        else
            return;
    }
    
    // read the control file.
    public boolean readControlFile()
    {
        File file=new File(gPathName);  
        FileInputStream input_stream=null;
        
        try {
          if(file.exists())
          {
                input_stream = new FileInputStream(gPathName);
            
                BufferedReader in=new BufferedReader(new InputStreamReader(input_stream));   
                String line=null;
                String ctrl_line = null;
                while((line=in.readLine())!=null)
                {   
                    if(line.equals(g_webcam_ctrl_str))
                    {
                        ctrl_line=in.readLine();
                        g_webcam_ctrl = Integer.parseInt(ctrl_line);
                        System.out.println(ctrl_line.toString());
                    }
                    else if(line.equals(g_fan_ctrl_str))
                    {
                        ctrl_line=in.readLine();
                        g_fan_ctrl = Integer.parseInt(ctrl_line);
                        System.out.println(ctrl_line.toString());
                    }
                    else if(line.equals(g_feed_ctrl_str))
                    {
                        ctrl_line=in.readLine();
                        g_feed_ctrl = Integer.parseInt(ctrl_line);
                        System.out.println(ctrl_line.toString());
                    }
                    else
                    {
                        System.out.println("readControlFile() not recognize the line.");
                    }
                    System.out.println(line.toString());
                    
                }   
            }
          else
          {
              System.out.println("Error: file not exist.");
              return false;
          }
        } catch (MalformedURLException e) {   
          e.printStackTrace();   
        } catch (IOException e) {   
          e.printStackTrace();   
        }
        finally {   
            try {   
                input_stream.close();   
                System.out.println("readControlFile() input close success");   
            } catch (IOException e) {   
                System.out.println("readControlFile() input close fail");   
                e.printStackTrace();   
            }   
        }   
        
        return true;
    }
    
 // overwrite the control file.
    public boolean writeControlFile()
    {
        File file=new File(gPathName);  
        OutputStream output_stream=null;
        
        try {
          if(file.exists())
          {
              output_stream = new FileOutputStream(gPathName);
              String cur_ctrl_line = null;
              String tmp_line=null;
              
              tmp_line = g_webcam_ctrl_str+"\n";
              output_stream.write(tmp_line.getBytes());
              cur_ctrl_line = Integer.toString(g_webcam_ctrl);
              tmp_line = cur_ctrl_line + "\n\n";
              output_stream.write(tmp_line.getBytes());
              
              tmp_line = g_fan_ctrl_str + "\n";
              output_stream.write(tmp_line.getBytes());
              cur_ctrl_line = Integer.toString(g_fan_ctrl);
              tmp_line = cur_ctrl_line+ "\n\n";
              output_stream.write(tmp_line.getBytes());
              
              tmp_line = g_feed_ctrl_str + "\n";
              output_stream.write(tmp_line.getBytes());
              cur_ctrl_line = Integer.toString(g_feed_ctrl);
              tmp_line = cur_ctrl_line + "\n\n";
              output_stream.write(tmp_line.getBytes());
  
              output_stream.flush();
          }
          else
          {
              System.out.println("Error: file not exist.");
              return false;
          }
        } catch (MalformedURLException e) {   
          e.printStackTrace();   
        } catch (IOException e) {   
          e.printStackTrace();   
        }
        finally {   
            try {   
                //input_stream.close();   
                output_stream.close();
                System.out.println("writeControlFile() close output success");   
            } catch (IOException e) {   
                System.out.println("writeControlFile() close output fail");   
                e.printStackTrace();   
            }   
        }   
        
        return true;
    }
    
 // download the control file from web.
    public void downloadControlFile() 
    {
        File file=new File(gPathName);   
        OutputStream output=null;
        
        try {   
            /*  
             * ͨ��URLȡ��HttpURLConnection  
             * Ҫ�������ӳɹ�������AndroidMainfest.xml�н���Ȩ������  
             * <uses-permission android:name="android.permission.INTERNET" />  
             */  
           
            URL url=new URL(gUrlStr);   
            HttpURLConnection conn=(HttpURLConnection)url.openConnection();   
            //ȡ��inputStream�������ж�ȡ   
            InputStream input=conn.getInputStream();   //maxSdkVersion must below 8
           
          
            //if(file.exists()){   
            //    System.out.println("File exits");   
            //    return;   
           //} else {
                file.createNewFile(); 
                output=new FileOutputStream(file);   
                //��ȡ���ļ�   
                byte[] buffer=new byte[256];   
                while(input.read(buffer)!=-1){   
                    output.write(buffer);   
                }   
                output.flush();   

            //}
       } catch (MalformedURLException e) {   
            e.printStackTrace();   
        } catch (IOException e) {   
            e.printStackTrace();   
        }
        finally{   
            try {   
                output.close();   
                System.out.println("success");   
            } catch (IOException e) {   
                System.out.println("fail");   
                e.printStackTrace();   
            }   
        }   
    }
    
    public void uploadControlFile() 
    {
        String urlStr="184.168.232.1";
        String port = "21";
        String username = "timonkun";
        String pwd = "SAw7895123!";
        String remotePath = "/webcam/";
        String fileNamePath = gDirName;   //"/data/media/Download/";
        String fileName = gFileName;
        boolean ret = false;
        
        ret = ftpUpload(urlStr, port, username, pwd, remotePath, fileNamePath, fileName);
        if(!ret)
            System.out.println("Error: FTP upload fail.");
    }
    
    /**  
     * ͨ��ftp�ϴ��ļ�  
     * @param url ftp��������ַ �磺 192.168.1.110  
     * @param port �˿��� �� 21  
     * @param username  ��¼��  
     * @param password   ����  
     * @param remotePath  �ϵ�ftp�������Ĵ���·��  
     * @param fileNamePath  Ҫ�ϴ����ļ�·��  
     * @param fileName      Ҫ�ϴ����ļ���  
     * @return  
     */   
    private boolean ftpUpload(String url, String port, String username,String password, String remotePath, String fileNamePath,String fileName) {   
     FTPClient ftpClient = new FTPClient();   
     FileInputStream fis = null;   
     boolean ret = false;   
     try {   
         ftpClient.connect(url, Integer.parseInt(port));   
         boolean loginResult = ftpClient.login(username, password);   
         int returnCode = ftpClient.getReplyCode();   
         if (loginResult && FTPReply.isPositiveCompletion(returnCode)) {// �����¼�ɹ�    
             ftpClient.makeDirectory(remotePath);   
             // �����ϴ�Ŀ¼    
             ftpClient.changeWorkingDirectory(remotePath);   
             //ftpClient.setBufferSize(1024);   
            // ftpClient.setControlEncoding("UTF-8");   
             ftpClient.enterLocalPassiveMode();   
             fis = new FileInputStream(fileNamePath + fileName);   
             ret = ftpClient.storeFile(fileName, fis);   
                
             //ret = true;   //�ϴ��ɹ�          
         } else {// �����¼ʧ��    
             ret = false;   
         }   
     } catch (IOException e) {   
         e.printStackTrace();   
         throw new RuntimeException("Error: FTP client upload fail.", e);   
     } finally {   
         //IOUtils.closeQuietly(fis);    
     try {
         fis.close();
         ftpClient.disconnect();   
     } catch (IOException e) {   
            e.printStackTrace();   
            throw new RuntimeException("Error: FTP close exception.", e);   
        }   
     }   
     return ret;   
    }  

}
