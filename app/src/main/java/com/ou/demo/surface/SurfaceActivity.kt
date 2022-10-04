package com.ou.demo.surface

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.ou.demo.databinding.ActivitySurfaceBinding

class SurfaceActivity: AppCompatActivity() {

    private lateinit var binding:ActivitySurfaceBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivitySurfaceBinding.inflate(layoutInflater)
        setContentView(binding.root)
    }
}