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
        renderMode = RENDERMODE_WHEN_DIRTY
    }

    inner class MyGlRender(val nativeRender: MyNativeRender): GLSurfaceView.Renderer {

        fun setType(type: Int) {
            nativeRender.native_init(type)
        }

        override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
            Util.log("onSurfaceCreate---- surface = ${holder.surface}")
            nativeRender.native_init(RenderType.Yuv.ordinal)
            nativeRender.native_onSurfaceCreate(holder.surface)
        }

        override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
            Util.log("onSurfaceChanged----")
            var bitmap = BitmapFactory.decodeResource(resources, R.mipmap.lyf)
            glNativeRender?.setBitmapData(bitmap)
            nativeRender.native_onSurfaceChanged(width, height)
        }

        override fun onDrawFrame(gl: GL10) {
            Util.log("onDrawFrame ----")
            nativeRender.native_onDrawFrame()
        }

    }

    enum class RenderType {
        Triangle,
        Texture,
        Vao,
        Yuv,
    }
}