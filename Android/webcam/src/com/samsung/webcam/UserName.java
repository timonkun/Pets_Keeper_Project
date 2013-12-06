package com.samsung.webcam;

public class UserName{
    private String user_name;
    private String cli_pub_ip;
    private int cli_pub_port;
    private String private_ip;
    private int private_port;
    
    public UserName()
    {
        this.user_name = null;
        this.cli_pub_ip = null;
        this.cli_pub_port = -1;
        this.private_ip = null;
        this.private_port = -1;
    }
    
    public UserName(String userNameParam, String cliPubIP, String cliPubPort, String PrivateIP, String PrivatePort)
    {
        this.user_name = userNameParam;
        this.cli_pub_ip = cliPubIP;
        this.cli_pub_port = Integer.parseInt(cliPubPort);
        this.private_ip = PrivateIP;
        this.private_port = Integer.parseInt(PrivatePort);
    }
    
    public boolean isNull()
    {
        if (this.user_name.equals(null))
            return true;
        else return false;
    }
    
    public void setAllParams(String userNameParam, String cliPubIP, String cliPubPort, String PrivateIP, String PrivatePort)
    {
        this.user_name = userNameParam;
        this.cli_pub_ip = cliPubIP;
        this.cli_pub_port = Integer.parseInt(cliPubPort);
        this.private_ip = PrivateIP;
        this.private_port = Integer.parseInt(PrivatePort);
    }
     
    public void setUserName(String userNameParam)
    {
        this.user_name = userNameParam;
    }
    
    public String getUserName()
    {
        return this.user_name;
    }
    
    public void setCliPubIP(String CliPubIPParam)
    {
        this.cli_pub_ip = CliPubIPParam;
    }
    
    public String getCliPubIP()
    {
        return this.cli_pub_ip;
    }
    
    public void setCliPubPort(String cliPubPortParam)
    {
        this.cli_pub_port = Integer.parseInt(cliPubPortParam);
    }
    
    public void setCliPubPort(int cliPubPortParam)
    {
        this.cli_pub_port = cliPubPortParam;
    }
    
    public int getCliPubPort()
    {
        return this.cli_pub_port;
    }
    
    public void setPrivateIP(String privateIPParam)
    {
        this.private_ip = privateIPParam;
    }
    
    public String getPrivateIP()
    {
        return this.private_ip;
    }
    
    public void setPrivatePort(String privatePortParam)
    {
        this.private_port = Integer.parseInt(privatePortParam);
    }
    
    public void setPrivatePort(int privatePortParam)
    {
        this.private_port =  privatePortParam;
    }
    
    public int getPrivatePort()
    {
        return this.private_port;
    }
    
    public void printUser()
    {
        String buf = this.user_name + " " + this.cli_pub_ip + " " + this.cli_pub_port + " " + this.private_ip + " " + this.private_port;
        System.out.println(buf);
    }
}
