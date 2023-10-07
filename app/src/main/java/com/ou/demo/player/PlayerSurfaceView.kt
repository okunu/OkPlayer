package com.ou.demo.player

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.Surface
import com.ou.demo.util.Util
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class PlayerSurfaceView(context: Context, attr: AttributeSet) : GLSurfaceView(context, attr) {

    init {
        setEGLContextClientVersion(3)
        setEGLConfigChooser(8,8,8,8,16,8)
        setRenderer(PlayerGlRender())
    }

    inner class PlayerGlRender(): GLSurfaceView.Renderer {

        override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
            Util.log("onSurfaceCreate---- thread = ${Thread.currentThread().name}")
            surfaceCreate(NativePlayer2.instance.getRef(), holder.surface)
        }

        override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
            Util.log("onSurfaceChanged----thread = ${Thread.currentThread().name}")
            surfaceChange(NativePlayer2.instance.getRef(), width, height)
        }

        override fun onDrawFrame(gl: GL10) {
        }
    }

    external fun surfaceCreate(ref: Long, surface: Surface)
    external fun surfaceChange(ref: Long, width: Int, heiht: Int)
}