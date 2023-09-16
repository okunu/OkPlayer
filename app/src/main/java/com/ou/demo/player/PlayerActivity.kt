package com.ou.demo.player

import android.os.Bundle
import android.os.FileUtils
import android.util.Log
import android.view.Window
import androidx.appcompat.app.AppCompatActivity
import com.ou.demo.databinding.ActivityPlayerLayoutBinding
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
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
//            GlobalScope.launch {
//                val path = checkFile("res/yongqi.mp4")
//                player.playVideo(path, binding.surfaceView.holder.surface)
//            }
//            GlobalScope.launch {
//                val path = checkFile("res/yongqi.mp4")
//                player.playAudio(path)
//            }
            val path = checkFile("res/yongqi.mp4")
            player.play(path, binding.surfaceView.holder.surface)
        }
    }

    fun checkFile(assetPath: String): String {
        val fileName = assetPath.substring(assetPath.indexOf("/") + 1)
        val desPath = filesDir.absolutePath + File.separator + fileName
        try {
            assets.open(assetPath).use {
                FileOutputStream(desPath).use { out ->
                    FileUtils.copy(it, out)
                }
            }
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
    val path = "res/yongqi.mp4"
    val fileName = path.substring(path.indexOf("/") + 1)
    println(fileName)
}