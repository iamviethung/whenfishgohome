package com.android.trainingfw;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;
import android.view.Display;
import android.util.Log;

public class GameRenderer implements GLSurfaceView.Renderer {
	int m_iWidth;
	int m_iHeight;
	
	public GameRenderer() {
		// TODO Auto-generated constructor stub
		Display display = GameActivity.mInstance.getWindowManager().getDefaultDisplay();
		m_iWidth = display.getWidth();
		m_iHeight = display.getHeight();
	}
	
    public void onDrawFrame(GL10 gl) {
        nativeRender();
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
        nativeResize(width, height);
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // Init game
		Log.i("GAME", "Java - Init game");
    	nativeInit(m_iWidth, m_iHeight);
		Log.i("GAME", "Init Done game");
    }
    
    public static native void nativeInit(int width, int height);
    private static native void nativeResize(int width, int height);
    private static native void nativeRender();
}
