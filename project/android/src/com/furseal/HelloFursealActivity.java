/*
 * HelloFursealActivity.java
 *
 *  Created on: 2011-3-17
 *      Author: Lional King
 *
 *   Copyright (c) 2011 netsurfers
*/


package com.furseal;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.app.Activity;
import android.os.Bundle;


public class HelloFursealActivity extends Activity
{
    private FursealView m_catcake = new FursealView("hello_furseal");

    @Override public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        copyAssets();
        m_catcake.onCreate(this);
    }

    @Override public void onPause()
    {
        super.onPause();
        m_catcake.onPause();
    }

    @Override public void onResume()
    {
        super.onResume();
        m_catcake.onResume();
    }

    @Override public void onDestroy()
    {
        super.onDestroy();
        m_catcake.onDestroy();
    }
    
    private void copyAssets() {
        String[] files;
        File mWorkingPath = new File(getResources().getString(R.string.data_path));
        try {
                files = this.getResources().getAssets().list("");
        } catch (IOException e) {
                e.printStackTrace();
                return;
        }
            if(!mWorkingPath.exists()) {
                if(!mWorkingPath.mkdirs()) {
                        return;
                }
        }
        
        for(int i = 0; i < files.length; i++) {
                try {
                        String fileName = files[i];
                        if(fileName.compareTo("images") == 0 || 
                        fileName.compareTo("sounds") == 0 || 
                        fileName.compareTo("webkit") == 0)
                                continue;
                        File outFile = new File(mWorkingPath, fileName);
                        if(outFile.exists()) continue;
                        InputStream in = getAssets().open(fileName);
                        OutputStream out = new FileOutputStream(outFile);
                        
                        // Transfer bytes from in to out
                        byte[] buf = new byte[1024];
                        int len;
                        while ((len = in.read(buf)) > 0) {
                                out.write(buf, 0, len);
                        }
                        
                        in.close();
                        out.close();
                } catch (FileNotFoundException e) {
                        e.printStackTrace();   
                } catch (IOException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                    }
        }
}
}
