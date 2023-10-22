package com.ou.demo.player

import android.content.Context
import android.util.AttributeSet
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.ou.demo.util.Util

class PlayerSurfaceView(context: Context, attr: AttributeSet) : SurfaceView(context, attr) {

    init {
        holder.addCallback(PlayerGlRender())
    }

    inner class PlayerGlRender(): SurfaceHolder.Callback {
        override fun surfaceCreated(holder: SurfaceHolder) {
            Util.log("onSurfaceCreate---- thread = ${Thread.currentThread().name}")
        }

        override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
            Util.log("surfaceChanged---- thread = ${Thread.currentThread().name}")
            surfaceChange(NativePlayer2.instance.getRef(), width, height, holder.surface)
        }

        override fun surfaceDestroyed(holder: SurfaceHolder) {
            Util.log("surfaceDestroyed---- thread = ${Thread.currentThread().name}")
        }
    }

    external fun surfaceChange(ref: Long, width: Int, heiht: Int, surface: Surface)
}