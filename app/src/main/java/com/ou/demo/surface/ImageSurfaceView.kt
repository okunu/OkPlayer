package com.ou.demo.surface

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Paint
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.ou.demo.R

class ImageSurfaceView(context: Context, attrs: AttributeSet?) : SurfaceView(context, attrs), SurfaceHolder.Callback {

    init {
        var holder = holder
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        if (holder == null) {
            return
        }
        var paint = Paint()
        paint.isAntiAlias = true
        paint.style = Paint.Style.STROKE
        var bitmap = BitmapFactory.decodeResource(context.resources, R.mipmap.jj)
        var canvas = holder.lockCanvas()
        canvas.drawBitmap(bitmap, 0f, 0f, paint)
        holder.unlockCanvasAndPost(canvas)
    }

    override fun surfaceChanged(holder: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
    }
}