package com.ou.demo.opengl2

import android.os.Bundle
import android.view.View
import android.view.Window
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isVisible
import com.ou.demo.databinding.ActivityOpengl2Binding
import com.ou.demo.util.Common

class OpenGl2Activity: AppCompatActivity() {

    lateinit var binding: ActivityOpengl2Binding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE)
        binding = ActivityOpengl2Binding.inflate(layoutInflater)
        setContentView(binding.root)
        setDirectionVisible(false)
    }

    override fun onPause() {
        super.onPause()
        binding.yuv.onPause()
    }

    override fun onResume() {
        super.onResume()
        binding.yuv.onResume()
    }

    fun setDirectionVisible(show: Boolean) {
        binding.upButton.isVisible = show
        binding.leftButton.isVisible = show
        binding.rightButton.isVisible = show
        binding.downBtn.isVisible = show
        if (show) {
            binding.upButton.setOnClickListener {
                binding.yuv.changeDirection(Common.Direction.Up.ordinal)
            }
            binding.leftButton.setOnClickListener {
                binding.yuv.changeDirection(Common.Direction.Left.ordinal)
            }
            binding.rightButton.setOnClickListener {
                binding.yuv.changeDirection(Common.Direction.Right.ordinal)
            }
            binding.downBtn.setOnClickListener {
                binding.yuv.changeDirection(Common.Direction.Down.ordinal)
            }
        }
    }
}