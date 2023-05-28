package com.ou.demo.opengl2

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.util.Log
import android.view.MotionEvent
import com.ou.demo.render.MyNativeRender
import com.ou.demo.util.Common
import com.ou.demo.util.Util
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MySurfaceView(context: Context, attributeSet: AttributeSet): GLSurfaceView(context, attributeSet) {

    val TAG = "MySurfaceView"
    var glRender: MyGlRender? = null

    var glNativeRender: MyNativeRender? = null

    init {
        setEGLContextClientVersion(3)
        glNativeRender = MyNativeRender().also {
            glRender = MyGlRender(it)
        }
        setEGLConfigChooser(8,8,8,8,16,8);
        setRenderer(glRender)
        //不设置这个就会不停地刷新，设置这个就会有需要再刷新，比如uniform时就需要取消这个设置
//        renderMode = RENDERMODE_WHEN_DIRTY
    }

    fun changeDirection(direction: Int) {
        glNativeRender?.changeDirection(direction)
    }

    var lastX = 0f
    var lastY = 0f
    var last2x = 0f
    var last2y = 0f
    var canRotate = true
    var canScale = false

    override fun onTouchEvent(event: MotionEvent): Boolean {
        when(event.actionMasked) {
            MotionEvent.ACTION_DOWN ->{
                lastX = event.x
                lastY = event.y
                canRotate = true
                canScale = false
                Log.i(TAG, "onTouchEvent: down x = $lastX y = $lastY")
            }
            MotionEvent.ACTION_POINTER_DOWN ->{
                canRotate = false
                canScale = true
                if (event.pointerCount > 2) {
                    canScale = false
                    return false
                }
                var index0 = event.getPointerId(0)
                var index1 = event.getPointerId(1)
                lastX = event.getX(index0)
                lastY = event.getY(index0)
                last2x = event.getX(index1)
                last2y = event.getY(index1)
                Log.i(TAG, "onTouchEvent: down x = $lastX y = $lastY")
                glNativeRender?.rorate(0f, 0f, 0f)
            }
            MotionEvent.ACTION_POINTER_UP ->{
                canRotate = false
                canScale = false
                glNativeRender?.rorate(0f, 0f, 0f)
                Log.i(TAG, "onTouchEvent: ACTION_POINTER_UP")
            }
            MotionEvent.ACTION_MOVE ->{
                if (canRotate) {
                    if (event.pointerCount > 2) {
                        glNativeRender?.rorate(0f, 0f, 0f)
                        return true
                    }
                    var curX = event.x
                    var curY = event.y
                    var xoffset = curX - lastX
                    var yoffset = curY - lastY
                    lastX = curX
                    lastY = curY
                    var distance = Math.sqrt((xoffset * xoffset).toDouble() + (yoffset * yoffset).toDouble())
                    glNativeRender?.rorate(xoffset, yoffset, distance.toFloat())
                } else if (canScale) {
                    val index0 = event.getPointerId(0)
                    val index1 = event.getPointerId(1)
                    val curX = event.getX(index0)
                    val curY = event.getY(index0)
                    val cur1X = event.getX(index1)
                    val cur1Y = event.getY(index1)
                    val lastDis=Math.sqrt(Math.pow((lastX-last2x).toDouble(), 2.0)+Math.pow((lastY-last2y).toDouble(), 2.0))
                    val curDis=Math.sqrt(Math.pow((curX-cur1X).toDouble(), 2.0)+Math.pow((curY-cur1Y).toDouble(), 2.0))
                    glNativeRender?.scale((curDis/lastDis).toFloat())
                }
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
            nativeRender.native_init(Common.RenderType.Blend.ordinal)
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