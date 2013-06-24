package com.samsung.webcam;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
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
    private Webcam webcam = new Webcam();
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        webcam.downloadControlFile();
        webcam.readControlFile();
        
        toggleWebcamBtn = (ToggleButton) findViewById(R.id.toggleWebcamBtn);  
        toggleWebcamBtn.setChecked(webcam.getWebcamCtrl());
        
        toggleFanBtn = (ToggleButton) findViewById(R.id.toggleFanBtn);  
        toggleFanBtn.setChecked(webcam.getFanCtrl());
        
        toggleFeedBtn = (ToggleButton) findViewById(R.id.toggleFeedBtn);  
        toggleFeedBtn.setChecked(webcam.getFeedCtrl());
        
        toggleWebcamBtn.setOnClickListener(new OnClickListener() {     
        public void onClick(View v) {         
               // 当按钮第一次被点击时候响应的事件      
             if (toggleWebcamBtn.isChecked()) {    
                 webcam.setWebcamCtrl(1);
                 webcam.writeControlFile();
                 webcam.uploadControlFile();
                 Toast.makeText(MainActivity.this, "webcam on", Toast.LENGTH_SHORT).show();     
             }  
             // 当按钮再次被点击时候响应的事件  
             else {         
                 webcam.setWebcamCtrl(0);
                 webcam.writeControlFile();
                 webcam.uploadControlFile();
                 Toast.makeText(MainActivity.this, "webcam off", Toast.LENGTH_SHORT).show();       
             }     
        } 
        }); 
        
        
        toggleFanBtn.setOnClickListener(new OnClickListener() {     
        public void onClick(View v) {         
               // 当按钮第一次被点击时候响应的事件      
             if (toggleWebcamBtn.isChecked()) {    
                 webcam.setFanCtrl(1);
                 webcam.writeControlFile();
                 webcam.uploadControlFile();
                 Toast.makeText(MainActivity.this, "Fan on", Toast.LENGTH_SHORT).show();     
             }  
             // 当按钮再次被点击时候响应的事件  
             else {         
                 webcam.setFanCtrl(0);
                 webcam.writeControlFile();
                 webcam.uploadControlFile();
                 Toast.makeText(MainActivity.this, "Fan off", Toast.LENGTH_SHORT).show();       
             }     
        } 
        }); 
        
        
        toggleFeedBtn.setOnClickListener(new OnClickListener() {     
        public void onClick(View v) {         
               // 当按钮第一次被点击时候响应的事件      
             if (toggleWebcamBtn.isChecked()) {    
                 webcam.setFeedCtrl(1);
                 webcam.writeControlFile();
                 webcam.uploadControlFile();
                 Toast.makeText(MainActivity.this, "Feed on", Toast.LENGTH_SHORT).show();     
             }  
             // 当按钮再次被点击时候响应的事件  
             else {         
                 webcam.setFeedCtrl(0);
                 webcam.writeControlFile();
                 webcam.uploadControlFile();
                 Toast.makeText(MainActivity.this, "Feed off", Toast.LENGTH_SHORT).show();       
             }     
        } 
        }); 
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }

    
}
