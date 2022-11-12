package com.ou.demo.opengl2

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.ou.demo.databinding.ActivityOpengl2Binding

class OpenGl2Activity: AppCompatActivity() {

    lateinit var binding: ActivityOpengl2Binding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityOpengl2Binding.inflate(layoutInflater)
        setContentView(binding.root)
    }
}