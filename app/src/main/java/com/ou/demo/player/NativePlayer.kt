package com.ou.demo.player

import android.media.AudioFormat
import android.media.AudioManager
import android.media.AudioTrack
import android.view.Surface

class NativePlayer {

    private var audioTrack: AudioTrack? = null

    private var nativeRef: Long = 0

    fun createAudioTrack(sampleRate: Int, channels: Int) {
        var channelConfig: Int = 0
        if (channels == 1) {
            channelConfig = AudioFormat.CHANNEL_OUT_MONO
        } else if (channels == 2) {
            channelConfig = AudioFormat.CHANNEL_OUT_STEREO
        } else {
            channelConfig = AudioFormat.CHANNEL_OUT_MONO
        }
        val bufferSize =
            AudioTrack.getMinBufferSize(sampleRate, channelConfig, AudioFormat.ENCODING_PCM_16BIT)
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

    init {
        nativeRef = init_player()
    }

    fun realPlay(_path: String, surface: Surface) {
        if (nativeRef != 0L) {
            real_play(nativeRef, _path, surface)
        }
    }

    fun realPlayOrPause() {
        if (nativeRef != 0L) {
            play_or_pause(nativeRef)
        }
    }

    external fun playVideo(path: String, surface: Surface)

    external fun playAudio(path: String)

    external fun play(_path: String, surface: Surface)

    external fun init_player(): Long

    external fun real_play(ref: Long, _path: String, surface: Surface)

    external fun play_or_pause(ref: Long)
}