package com.ou.demo.nativecontext

import android.content.res.AssetManager
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import com.ou.demo.App

class NativeContext {

    companion object {
        val instance: NativeContext by lazy { NativeContext() }

        //必须要加这个注解，否则会crash，反射会找不到这个方法
        @JvmStatic
        fun getAssetBitmap(path: String): Bitmap {
            val ins = App.context.assets.open(path)
            val bitmap = BitmapFactory.decodeStream(ins)
            return bitmap
        }
    }

    external fun setAssets(assetManager: AssetManager)
}