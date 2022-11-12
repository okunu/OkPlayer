package com.ou.demo.opengl

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.ou.demo.databinding.ActivityOpenglBinding

class OpenGlActivity: AppCompatActivity() {

    lateinit var binding: ActivityOpenglBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityOpenglBinding.inflate(layoutInflater)
        setContentView(binding.root)
    }
}