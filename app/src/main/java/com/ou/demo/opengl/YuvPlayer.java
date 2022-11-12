package com.ou.demo.opengl;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;

import com.ou.demo.util.Util;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class YuvPlayer extends GLSurfaceView implements Runnable, SurfaceHolder.Callback, GLSurfaceView.Renderer {

    public YuvPlayer(Context context, AttributeSet attrs) {
        super(context, attrs);
        setRenderer(this);
        setRenderMode(RENDERMODE_CONTINUOUSLY);
        Util.INSTANCE.log("init surface = " + getHolder().getSurface());
    }

    @Override
    public void run() {
        drawTriangle(getHolder().getSurface());
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.d("okunu","surfaceCreated ---- ");
        Util.INSTANCE.log("surface = " + getHolder().getSurface());
        //在Surface创建的时候启动绘任务
        new Thread(this).start();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {

    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.d("okunu","onSurfaceCreated");
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.d("okunu","onSurfaceChanged");
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        Log.d("okunu","onDrawFrame");
    }

    public native void drawTriangle(Object surface);
}
