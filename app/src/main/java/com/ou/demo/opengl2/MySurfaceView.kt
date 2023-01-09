package com.ou.demo.opengl2

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.MotionEvent
import com.ou.demo.render.MyNativeRender
import com.ou.demo.util.Common
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
//        renderMode = RENDERMODE_WHEN_DIRTY
    }

    fun changeDirection(direction: Int) {
        glNativeRender?.changeDirection(direction)
    }

    var lastX = getContext().resources.displayMetrics.widthPixels/2f
    var lastY = getContext().resources.displayMetrics.heightPixels /2f
    var yaw = 0f
    var pitch = 0f
    override fun onTouchEvent(event: MotionEvent): Boolean {
        when(event.action) {
            MotionEvent.ACTION_DOWN ->{
                lastX = event.x
                lastY = event.y
            }
            MotionEvent.ACTION_MOVE ->{
                var curX = event.x
                var curY = event.y
                var xoffset = curX - lastX
                var yoffset = curY - lastY
                lastX = curX
                lastY = curY

                var sensitivity = 0.15f
                xoffset *= sensitivity
                yoffset *= sensitivity

                yaw += xoffset
                pitch += yoffset
                if(pitch > 89.0f)
                    pitch = 89.0f
                if(pitch < -89.0f)
                    pitch = -89.0f
                glNativeRender?.setAngle(yaw, pitch)
                Util.log("move yaw = $yaw  pitch = $pitch xoffset = $xoffset yoofset = $yoffset")
            }
        }
        return true
    }

    inner class MyGlRender(val nativeRender: MyNativeRender): GLSurfaceView.Renderer {

        fun setType(type: Int) {
            nativeRender.native_init(type)
        }

        override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
            Util.log("onSurfaceCreate---- thread = ${Thread.currentThread().name}")
            nativeRender.native_init(Common.RenderType.ComplexCube.ordinal)
            nativeRender.native_onSurfaceCreate(holder.surface)
        }

        override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
            Util.log("onSurfaceChanged----thread = ${Thread.currentThread().name}")
            nativeRender.native_onSurfaceChanged(width, height)
        }

        override fun onDrawFrame(gl: GL10) {
//            Util.log("onDrawFrame ----thread = ${Thread.currentThread().name}")
            nativeRender.native_onDrawFrame()
        }

    }
}