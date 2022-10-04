package com.ou.demo.audio

import android.Manifest
import android.content.pm.PackageManager
import android.media.AudioFormat
import android.media.AudioManager
import android.media.AudioRecord
import android.media.AudioTrack
import android.media.MediaRecorder
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.ou.demo.databinding.ActivityRecordBinding
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.cancel
import kotlinx.coroutines.launch
import java.io.File
import java.io.FileInputStream
import java.io.FileOutputStream

class AudioActivity: AppCompatActivity(), CoroutineScope by CoroutineScope(Dispatchers.Default) {

    private lateinit var binding:ActivityRecordBinding
    private val REQUEST_CODE = 1025
    private var mAudioRecord: AudioRecord? = null
    private var mIsRecording = false
    private var mBufferSize = 0
    private var mAudioTrack: AudioTrack? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityRecordBinding.inflate(layoutInflater)
        setContentView(binding.root)
        requestPermission()
        binding.recordBtn.setOnClickListener { startRecord() }
        binding.stopRecordBtn.setOnClickListener { stopRecord() }
        binding.playBtn.setOnClickListener { play() }
    }

    private fun checkExternalStorageReady(): Boolean {
        return Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)
    }

    fun createAudioRecord() {
        mBufferSize = AudioRecord.getMinBufferSize(44100, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT)
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
            return
        }
        mAudioRecord = AudioRecord(MediaRecorder.AudioSource.MIC, 44100, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT, mBufferSize)
    }

    fun startRecord() {
        if (mIsRecording) {
            return
        }
        if (!checkExternalStorageReady()) {
            return
        }
        if (mAudioRecord == null) {
            createAudioRecord()
        }
        var fileName = Environment.getExternalStorageDirectory().absolutePath + File.separator + "at" + File.separator + "demo.pcm"
        mIsRecording = true
        mAudioRecord!!.startRecording()
        launch(context = Dispatchers.IO) {
            var byteBuffer = ByteArray(mBufferSize)
            var file = File(fileName)
            try {
                if (!file.exists()) {
                    file.createNewFile()
                }
                var fos = FileOutputStream(file)
                var read = -1
                while (mIsRecording) {
                    read = mAudioRecord!!.read(byteBuffer, 0, mBufferSize)
                    if (AudioRecord.ERROR_INVALID_OPERATION != read) {
                        fos.write(byteBuffer)
                    }
                }
                fos.close()
            } catch (e: java.lang.Exception){
                Log.e("okunu", "record", e)
            }
        }
    }

    fun play() {
        if (mAudioTrack == null) {
            mBufferSize = AudioRecord.getMinBufferSize(44100, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT)
            mAudioTrack = AudioTrack(AudioManager.STREAM_MUSIC, 44100, AudioFormat.CHANNEL_OUT_MONO,
                AudioFormat.ENCODING_PCM_16BIT, mBufferSize, AudioTrack.MODE_STREAM)
        }
        var fileName = Environment.getExternalStorageDirectory().absolutePath + File.separator + "at" + File.separator + "demo.pcm"
        var file = File(fileName)
        if (!file.exists()) {
            Log.i("okunu", "has no file")
            return
        }
        var buffer = ByteArray(mBufferSize)
        launch {
            mAudioTrack!!.play()
            try {
                var read = -1
                var fis = FileInputStream(file)
                do {
                    read = fis.read(buffer)
                    if (read == -1) {
                        break
                    }
                    mAudioTrack!!.write(buffer, 0, read)
                } while (true)
                mAudioTrack!!.stop()
                mAudioTrack = null
                fis.close()
            }catch (e: java.lang.Exception) {
                Log.e("okunu", "play", e)
            }
        }

    }

    fun stopRecord() {
        mIsRecording = false
        if (mAudioRecord != null) {
            mAudioRecord!!.stop()
            mAudioRecord!!.release()
            mAudioRecord = null
        }
    }

    fun requestPermission() {
        var hasPermission = ActivityCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED
        if (!hasPermission) {
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.RECORD_AUDIO), 1025)
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        cancel()
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == REQUEST_CODE){
            var hasPermission = ActivityCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED
            if (!hasPermission) {
                Toast.makeText(this, "has no record permission", Toast.LENGTH_SHORT).show()
            }
        }
    }
}