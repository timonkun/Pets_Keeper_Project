package com.samsung.webcam;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.Toast;
import android.widget.ToggleButton;
import android.support.v4.app.NavUtils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public class MainActivity extends Activity {

    private ToggleButton  toggleWebcamBtn;
    private ToggleButton  toggleFanBtn;
    private ToggleButton  toggleFeedBtn;
    private Button updateBtn;
    private Webcam webcam = new Webcam();
    private SocketUDPClient socketUDPClient = new SocketUDPClient();
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        //webcam.downloadControlFile();
        //webcam.readControlFile();
        
        toggleWebcamBtn = (ToggleButton) findViewById(R.id.toggleWebcamBtn);  
        toggleWebcamBtn.setChecked(webcam.getWebcamCtrl());
        
        toggleFanBtn = (ToggleButton) findViewById(R.id.toggleFanBtn);  
        toggleFanBtn.setChecked(webcam.getFanCtrl());
        
        toggleFeedBtn = (ToggleButton) findViewById(R.id.toggleFeedBtn);  
        toggleFeedBtn.setChecked(webcam.getFeedCtrl());
        
        updateBtn = (Button) findViewById(R.id.updateBtn);
        
        // Register
        toggleWebcamBtn.setOnClickListener(new OnClickListener() {     
        public void onClick(View v) {         
               // 当按钮第一次被点击时候响应的事件      
             if (toggleWebcamBtn.isChecked()) {    
                 //注册client
                 socketUDPClient.setupSocket();
                 //webcam.setWebcamCtrl(1);
                 //webcam.writeControlFile();
                 //webcam.uploadControlFile();
                 Toast.makeText(MainActivity.this, "webcam on", Toast.LENGTH_SHORT).show();     
             }  
             // 当按钮再次被点击时候响应的事件  
             else {    
                 //目前啥都不做，将来设计为logout
                 //webcam.setWebcamCtrl(0);
                 //webcam.writeControlFile();
                 //webcam.uploadControlFile();
                 Toast.makeText(MainActivity.this, "webcam off", Toast.LENGTH_SHORT).show();       
             }     
        } 
        }); 
        
        // FAN
        toggleFanBtn.setOnClickListener(new OnClickListener() {     
        public void onClick(View v) {         
               // 当按钮第一次被点击时候响应的事件      
             if (toggleFanBtn.isChecked()) {  
                 socketUDPClient.registerClient();
                 //webcam.setFanCtrl(1);
                 //webcam.writeControlFile();
                 //webcam.uploadControlFile();
                 Toast.makeText(MainActivity.this, "Fan on", Toast.LENGTH_SHORT).show();     
             }  
             // 当按钮再次被点击时候响应的事件  
             else {         
                 //webcam.setFanCtrl(0);
                 //webcam.writeControlFile();
                 //webcam.uploadControlFile();
                 Toast.makeText(MainActivity.this, "Fan off", Toast.LENGTH_SHORT).show();       
             }     
        } 
        }); 
        
        // FEED
        toggleFeedBtn.setOnClickListener(new OnClickListener() {     
        public void onClick(View v) {         
               // 当按钮第一次被点击时候响应的事件      
             if (toggleFeedBtn.isChecked()) {    
                 //webcam.setFeedCtrl(1);
                 //webcam.writeControlFile();
                 //webcam.uploadControlFile();
                 Toast.makeText(MainActivity.this, "Feed on", Toast.LENGTH_SHORT).show();     
             }  
             // 当按钮再次被点击时候响应的事件  
             else {         
                 //webcam.setFeedCtrl(0);
                 //webcam.writeControlFile();
                 //webcam.uploadControlFile();
                 Toast.makeText(MainActivity.this, "Feed off", Toast.LENGTH_SHORT).show();       
             }     
        } 
        }); 
        
        updateBtn.setOnClickListener(new OnClickListener(){
            
            public void onClick(View v) {
                // TODO Auto-generated method stub
                //webcam.downloadControlFile();
                //webcam.readControlFile();
                toggleWebcamBtn.setChecked(webcam.getWebcamCtrl());
                toggleFanBtn.setChecked(webcam.getFanCtrl());
                toggleFeedBtn.setChecked(webcam.getFeedCtrl());
            } 
            
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }

    
}
