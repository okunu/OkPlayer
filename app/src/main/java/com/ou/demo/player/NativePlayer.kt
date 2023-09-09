package com.ou.demo.player

import android.media.AudioFormat
import android.media.AudioManager
import android.media.AudioTrack
import android.view.Surface

class NativePlayer {

    private var audioTrack: AudioTrack? = null

    fun createAudioTrack(sampleRate: Int, channels: Int) {
        var channelConfig: Int = 0
        if (channels == 1) {
            channelConfig = AudioFormat.CHANNEL_OUT_MONO
        } else if (channels == 2) {
            channelConfig = AudioFormat.CHANNEL_OUT_STEREO
        } else {
            channelConfig = AudioFormat.CHANNEL_OUT_MONO
        }
        val bufferSize = AudioTrack.getMinBufferSize(sampleRate, channelConfig, AudioFormat.ENCODING_PCM_16BIT)
        audioTrack = AudioTrack(
            AudioManager.STREAM_MUSIC,
            sampleRate,
            channelConfig,
            AudioFormat.ENCODING_PCM_16BIT,
            bufferSize,
            AudioTrack.MODE_STREAM
        )
        audioTrack?.play()
    }

    fun playAudioTrack(data: ByteArray, length: Int) {
        audioTrack?.let {
            if (it.playState == AudioTrack.PLAYSTATE_PLAYING) {
                it.write(data, 0, length)
            }
        }
    }

    fun releaseAudioTrack() {
        audioTrack?.let {
            if (it.playState == AudioTrack.PLAYSTATE_PLAYING) {
                it.stop()
            }
            it.release()
        }
        audioTrack = null
    }

    external fun playVideo(path: String, surface: Surface)

    external fun playAudio(path: String)
}