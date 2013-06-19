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
 * 1. 启动后自动从网上下载脚本，根据脚本配置初始化开关按钮
    1.1 download
    1.2 read config
    1.3 initialize button
    1.4 适当的延迟反馈用户

   2. 用户操作开关后，更新配置脚本，上传网络
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
    private static final String gPathName="/data/media/Download/"+gFileName;//文件存储路径 
    private static final String gUrlStr="http://timonkun.me/webcam/control.txt";  
    
    public Webcam(){}
    
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
                StringBuffer sb=new StringBuffer();   
                while((line=in.readLine())!=null)
                {   
                    //sb.append(line);   
                    System.out.println(line.toString());
                    if(line.equals(g_webcam_ctrl_str))
                    {
                        ctrl_line=in.readLine();
                        g_webcam_ctrl = Integer.parseInt(ctrl_line);
                    }
                    else if(line.equals(g_fan_ctrl_str))
                    {
                        ctrl_line=in.readLine();
                        g_fan_ctrl = Integer.parseInt(ctrl_line);
                    }
                    else if(line.equals(g_feed_ctrl_str))
                    {
                        ctrl_line=in.readLine();
                        g_feed_ctrl = Integer.parseInt(ctrl_line);
                    }
                    else
                    {
                        //System.out.println("not recognize the line.");
                    }
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
                System.out.println("success");   
            } catch (IOException e) {   
                System.out.println("fail");   
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
              tmp_line = cur_ctrl_line + "\n";
              output_stream.write(tmp_line.getBytes());
              
              tmp_line = g_fan_ctrl_str + "\n";
              output_stream.write(tmp_line.getBytes());
              cur_ctrl_line = Integer.toString(g_fan_ctrl);
              tmp_line = cur_ctrl_line+ "\n";
              output_stream.write(tmp_line.getBytes());
              
              tmp_line = g_feed_ctrl_str + "\n";
              output_stream.write(tmp_line.getBytes());
              cur_ctrl_line = Integer.toString(g_feed_ctrl);
              tmp_line = cur_ctrl_line + "\n";
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
                System.out.println("success");   
            } catch (IOException e) {   
                System.out.println("fail");   
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
             * 通过URL取得HttpURLConnection  
             * 要网络连接成功，需在AndroidMainfest.xml中进行权限配置  
             * <uses-permission android:name="android.permission.INTERNET" />  
             */  
           
            URL url=new URL(gUrlStr);   
            HttpURLConnection conn=(HttpURLConnection)url.openConnection();   
            //取得inputStream，并进行读取   
            InputStream input=conn.getInputStream();   //maxSdkVersion must below 8
           
          
            //if(file.exists()){   
            //    System.out.println("File exits");   
            //    return;   
           //} else {
                file.createNewFile(); 
                output=new FileOutputStream(file);   
                //读取大文件   
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
        String fileNamePath = "/data/media/Download/";
        String fileName = "control.txt";
        String retMesg = null;
        
        retMesg = ftpUpload(urlStr, port, username, pwd, remotePath, fileNamePath, fileName);
        System.out.println(retMesg);
    }
    
    /**  
     * 通过ftp上传文件  
     * @param url ftp服务器地址 如： 192.168.1.110  
     * @param port 端口如 ： 21  
     * @param username  登录名  
     * @param password   密码  
     * @param remotePath  上到ftp服务器的磁盘路径  
     * @param fileNamePath  要上传的文件路径  
     * @param fileName      要上传的文件名  
     * @return  
     */   
    private String ftpUpload(String url, String port, String username,String password, String remotePath, String fileNamePath,String fileName) {   
     FTPClient ftpClient = new FTPClient();   
     FileInputStream fis = null;   
     String returnMessage = "0";   
     try {   
         ftpClient.connect(url, Integer.parseInt(port));   
         boolean loginResult = ftpClient.login(username, password);   
         int returnCode = ftpClient.getReplyCode();   
         if (loginResult && FTPReply.isPositiveCompletion(returnCode)) {// 如果登录成功    
             ftpClient.makeDirectory(remotePath);   
             // 设置上传目录    
             ftpClient.changeWorkingDirectory(remotePath);   
             //ftpClient.setBufferSize(1024);   
            // ftpClient.setControlEncoding("UTF-8");   
             ftpClient.enterLocalPassiveMode();   
                     fis = new FileInputStream(fileNamePath + fileName);   
             ftpClient.storeFile(fileName, fis);   
                
             returnMessage = "1";   //上传成功          
         } else {// 如果登录失败    
             returnMessage = "0";   
             }   
                    
       
     } catch (IOException e) {   
         e.printStackTrace();   
         throw new RuntimeException("FTP客户端出错！", e);   
     } finally {   
         //IOUtils.closeQuietly(fis);    
     try {   
         ftpClient.disconnect();   
     } catch (IOException e) {   
            e.printStackTrace();   
            throw new RuntimeException("关闭FTP连接发生异常！", e);   
        }   
     }   
     return returnMessage;   
    }  

}
