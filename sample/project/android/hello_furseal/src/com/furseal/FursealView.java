/*
 * FursealView.java
 *
 *  Created on: 2011-3-17
 *      Author: Lional King
 *
 *   Copyright (c) 2011 netsurfers
*/


package com.furseal;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;

import android.content.Context;
import android.view.KeyEvent;
import android.view.MotionEvent;


public class FursealView
{
    private FursealGLView m_view;
    private FursealViewRenderer m_renderer;

    public FursealView(String lib_name) {
        System.loadLibrary(lib_name);
    }

    public void onCreate(Activity activity) {
        activity.getWindow().addFlags(android.view.WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        activity.getWindow().addFlags(android.view.WindowManager.LayoutParams.FLAG_FULLSCREEN);
        activity.requestWindowFeature(android.view.Window.FEATURE_NO_TITLE);

        m_view = new FursealGLView(activity);
        m_renderer = new FursealViewRenderer();

        m_view.setRenderer(m_renderer);
        activity.setContentView(m_view);
    }

    public void onPause() {
        m_view.onPause();
    }

    public void onResume() {
        m_view.onResume();
    }

    public void onDestroy() {
        nativeFinalize();
    }

	public void onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		m_view.onKeyDown(keyCode, event);
	}

	public void onKeyUp(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		m_view.onKeyUp(keyCode, event);
	}

    private static native void nativeInitialize();
    private static native void nativeUpdate();
    private static native void nativeFinalize();
    private static native void nativePause();
    private static native void nativeResume();
    private static native void nativeTouch(int action, int x, int y);
    private static native void nativeOnKeyDown(int key_code);
    private static native void nativeOnKeyUp(int key_code);

    private class FursealGLView extends GLSurfaceView {
        public FursealGLView(Context context) {
            super(context);
        }

        @Override
        public void setRenderer(Renderer renderer) {
            super.setRenderer(renderer);
        }
        
        @Override
        public void onResume() {
        	// TODO Auto-generated method stub
        	super.onResume();
            nativeResume();
        }
        
        @Override
        public void onPause() {
        	// TODO Auto-generated method stub
        	super.onPause();
            nativePause();
        }

        @Override
        public boolean onTouchEvent(MotionEvent event) {
            nativeTouch(event.getAction(), (int)event.getX(), (int)event.getY());
            return super.onTouchEvent(event);
        }
        
        @Override
        public boolean onKeyDown(int keyCode, KeyEvent event) {
        	// TODO Auto-generated method stub
        	nativeOnKeyDown(keyCode);
        	return super.onKeyDown(keyCode, event);
        }
        
        @Override
        public boolean onKeyUp(int keyCode, KeyEvent event) {
        	// TODO Auto-generated method stub
        	nativeOnKeyUp(keyCode);
        	return super.onKeyUp(keyCode, event);
        }
    }

    private class FursealViewRenderer implements Renderer {
    	
        boolean m_is_first = true;
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {}
        public void onSurfaceChanged(GL10 gl, int width, int height) {}

        public void onDrawFrame(GL10 gl) {
            if (m_is_first) {
                m_is_first = false;
                nativeInitialize();
            }
            nativeUpdate();
        }
    }
}
