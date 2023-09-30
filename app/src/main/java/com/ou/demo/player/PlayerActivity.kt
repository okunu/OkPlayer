package com.ou.demo.player

import android.os.Bundle
import android.os.FileUtils
import android.util.Log
import android.view.Window
import androidx.appcompat.app.AppCompatActivity
import com.ou.demo.databinding.ActivityPlayerLayoutBinding
import java.io.File
import java.io.FileOutputStream

class PlayerActivity : AppCompatActivity() {

    lateinit var binding: ActivityPlayerLayoutBinding
    lateinit var player: NativePlayer

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE)
        binding = ActivityPlayerLayoutBinding.inflate(layoutInflater)
        setContentView(binding.root)
        player = NativePlayer()

        binding.btnVideo.setOnClickListener {
            checkFile("res/yongqi.mp4") { path ->
                Log.i(TAG, "start play $path")
                player.realPlay(path, binding.surfaceView.holder.surface)
            }
        }

        binding.btnPauseVideo.setOnClickListener {
            player.realPlayOrPause()
        }
    }

    fun checkFile(assetPath: String, action: (path: String) -> Unit): String {
        val fileName = assetPath.substring(assetPath.indexOf("/") + 1)
        val desPath = filesDir.absolutePath + File.separator + fileName
        if (File(desPath).exists()) {
            Log.i(TAG, "$desPath is exist")
            action(desPath)
            return desPath
        }
        try {
            if (File(desPath).parentFile?.exists() == false) {
                File(desPath).parentFile?.mkdirs()
            }
            Log.i(TAG, "copy file $desPath")
            assets.open(assetPath).use {
                FileOutputStream(desPath).use { out ->
                    FileUtils.copy(it, out)
                }
            }
            action(desPath)
        } catch (e: Exception) {
            Log.e(TAG, "checkFile $assetPath", e)
            return ""
        }
        return desPath
    }

    companion object {
        private const val TAG = "PlayerActivity"
    }
}

fun main() {
    val path = "res/video/yongqi.mp4"
    val fileName = path.substring(path.indexOf("/") + 1)
    println(fileName)
}