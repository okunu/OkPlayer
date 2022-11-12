package com.ou.demo

import android.app.Application
import android.content.Context
import com.ou.demo.nativecontext.NativeContext

class App: Application() {

    override fun onCreate() {
        super.onCreate()
        context = baseContext
        NativeContext.instance.setAssets(baseContext.assets)
    }

    companion object {
        // Used to load the 'demo' library on application startup.
        init {
            System.loadLibrary("demo")
        }
        lateinit var context: Context
    }
}