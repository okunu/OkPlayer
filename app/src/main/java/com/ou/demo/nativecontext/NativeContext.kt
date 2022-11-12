package com.ou.demo.nativecontext

import android.content.res.AssetManager

class NativeContext {

    companion object {
        val instance: NativeContext by lazy { NativeContext() }
    }

    external fun setAssets(assetManager: AssetManager)
}