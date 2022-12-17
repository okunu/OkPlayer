package com.ou.demo.opengl2

import android.content.Context
import android.graphics.BitmapFactory
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.Surface
import android.view.SurfaceHolder
import com.ou.demo.R
import com.ou.demo.render.MyNativeRender
import com.ou.demo.util.Util
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MySurfaceView(context: Context, attributeSet: AttributeSet): GLSurfaceView(context, attributeSet) {

    var glRender: MyGlRender? = null
    var glNativeRender: MyNativeRender? = null

    init {
        setEGLContextClientVersion(3)
        glNativeRender = MyNativeRender().also {
            glRender = MyGlRender(it)
        }
        setRenderer(glRender)
        //不设置这个就会不停地刷新，设置这个就会有需要再刷新，比如uniform时就需要取消这个设置
        renderMode = RENDERMODE_WHEN_DIRTY
    }

    inner class MyGlRender(val nativeRender: MyNativeRender): GLSurfaceView.Renderer {

        fun setType(type: Int) {
            nativeRender.native_init(type)
        }

        override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
            Util.log("onSurfaceCreate---- thread = ${Thread.currentThread().name}")
            nativeRender.native_init(RenderType.TwoTexture.ordinal)
            nativeRender.native_onSurfaceCreate(holder.surface)
        }

        override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
            Util.log("onSurfaceChanged----thread = ${Thread.currentThread().name}")
            nativeRender.native_onSurfaceChanged(width, height)
        }

        override fun onDrawFrame(gl: GL10) {
            Util.log("onDrawFrame ----thread = ${Thread.currentThread().name}")
            nativeRender.native_onDrawFrame()
        }

    }

    enum class RenderType {
        Triangle,
        Texture,
        Vao,
        Yuv,
        Uniform,
        TwoTexture,
    }
}