package com.ou.demo.opengl

import android.content.Context
import android.content.res.AssetManager
import com.ou.demo.util.Util.log
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.util.Log
import android.view.SurfaceHolder
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class YuvPlayer(context: Context?, attrs: AttributeSet?) : GLSurfaceView(context, attrs), Runnable,
    SurfaceHolder.Callback, GLSurfaceView.Renderer {
    init {
        setRenderer(this)
        renderMode = RENDERMODE_CONTINUOUSLY
        log("init surface = " + holder.surface)
    }

    override fun run() {
//        drawTriangle(holder.surface)
        drawYuv(holder.surface, context.assets)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        Log.d("okunu", "surfaceCreated ---- ")
        log("surface = " + getHolder().surface)
        //在Surface创建的时候启动绘任务
        Thread(this).start()
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {}
    override fun surfaceChanged(holder: SurfaceHolder, format: Int, w: Int, h: Int) {}
    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        Log.d("okunu", "onSurfaceCreated")
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        Log.d("okunu", "onSurfaceChanged")
    }

    override fun onDrawFrame(gl: GL10) {
        Log.d("okunu", "onDrawFrame")
    }

    external fun drawTriangle(surface: Any?)
    external fun drawYuv(surface: Any?, manager: AssetManager)
}