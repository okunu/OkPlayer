package com.ou.demo.surface

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.PorterDuff
import android.graphics.Rect
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView

class EcgSurfaceView(context: Context, attrs: AttributeSet?) : SurfaceView(context, attrs), SurfaceHolder.Callback {

    var ecgDatas: MutableList<Int> = java.util.ArrayList()
    var rect: Rect? = null
    var mCanvas: Canvas? = null
    var mPaint: Paint? = null

    val lineColor = "#01FC00"
    val waveSpeed = 30
    val sleepTime = 8

    var surfaceHolder: SurfaceHolder? = null
    var isCanRun = true
    var mStartX = 0
    var mStartY = 0

    private val drawRunnable = Runnable {
        while (isCanRun) {
            if (mStartX > 800){
                mStartX = 0
            }
            rect!!.set(mStartX, 0, mStartX + 16, 300)
            mCanvas = surfaceHolder!!.lockCanvas(rect)
            if (mCanvas == null) return@Runnable
            mCanvas!!.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR)
            if (ecgDatas.size > 0) {
                mCanvas!!.drawLine(mStartX.toFloat(), mStartY.toFloat(), (mStartX + 16).toFloat(), ecgDatas[0].toFloat(), mPaint!!)
                mStartX = mStartX + 16
                mStartY = ecgDatas[0]
                ecgDatas.removeAt(0)
            } else {
                initData()
            }
            surfaceHolder!!.unlockCanvasAndPost(mCanvas)
        }
    }

    init {
        surfaceHolder = this.holder
        surfaceHolder!!.addCallback(this)
        rect = Rect()
        mPaint = Paint()
        mPaint!!.setColor(Color.parseColor(lineColor))
        mPaint!!.isAntiAlias = true
        mPaint!!.strokeWidth = 2f
        initData()
    }

    fun initData() {
        ecgDatas.clear()
        for (i in 0..199) {
            ecgDatas.add((Math.random() * 200).toInt())
        }
        mStartY = ecgDatas[0]
    }


    override fun surfaceCreated(holder: SurfaceHolder) {
        Thread(drawRunnable).start()
    }

    override fun surfaceChanged(holder: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        isCanRun = false
    }
}