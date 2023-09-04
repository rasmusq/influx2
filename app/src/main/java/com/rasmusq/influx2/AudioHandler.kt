package com.rasmusq.influx2

import android.Manifest
import android.app.Activity
import android.content.Context
import android.content.pm.PackageManager
import android.media.AudioAttributes
import android.media.AudioFormat
import android.media.AudioTrack
import android.util.Log
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat

class AudioHandler(activity: Activity, onAudio: (inputBuffer: ShortArray, outputBuffer: ShortArray) -> Unit) {

    private var _audioTrack: AudioTrack? = null
    private var _audioThread: Thread? = null
    private var _keepLoopAlive: Boolean = false
    private var _playing: Boolean = false
    private val _audioFunc = onAudio

    private var _inputBuffer: ShortArray? = null
    private var _outputBuffer: ShortArray? = null

    init {
        checkAudioFeatures(activity)
        requestPermissions(activity)
    }

    fun createAudioTrack() {
        val bufferSize = 240
        val sampleRate = 48000

        _audioTrack = AudioTrack.Builder().setAudioAttributes(
            AudioAttributes.Builder()
                .setContentType(AudioAttributes.CONTENT_TYPE_UNKNOWN)
                .setUsage(AudioAttributes.USAGE_UNKNOWN).build()
        )
            .setAudioFormat(
                AudioFormat.Builder().setEncoding(AudioFormat.ENCODING_PCM_32BIT)
                    .setChannelMask(AudioFormat.CHANNEL_OUT_DEFAULT).setSampleRate(sampleRate).build()
            ).setPerformanceMode(AudioTrack.PERFORMANCE_MODE_LOW_LATENCY)
            .setBufferSizeInBytes(bufferSize).setTransferMode(AudioTrack.MODE_STREAM).build()

        _inputBuffer = ShortArray(bufferSize)
        _outputBuffer = ShortArray(bufferSize)
    }
    fun startPlayingAudio() {
        _audioTrack?.play()
        _keepLoopAlive = true
        _playing = true


        _audioThread = Thread {
            while (_keepLoopAlive) {
                if (_playing) {
                    _audioFunc(_inputBuffer!!, _outputBuffer!!)
                    _audioTrack?.write(_outputBuffer!!, 0, _outputBuffer!!.size, AudioTrack.WRITE_BLOCKING);
                }
            }
        }
        _audioThread?.start()
    }

    fun checkAudioFeatures(context: Context) {
        val hasLowLatencyFeature =
            context.packageManager.hasSystemFeature(PackageManager.FEATURE_AUDIO_LOW_LATENCY)
        val hasProFeature =
            context.packageManager.hasSystemFeature(PackageManager.FEATURE_AUDIO_PRO)
        Log.println(
            Log.VERBOSE,
            "AudioHandler",
            "Low latency feature: $hasLowLatencyFeature"
        )
        Log.println(Log.VERBOSE, "AudioHandler", "Pro feature: $hasProFeature")
    }

    fun requestPermissions(activity: Activity?) {
        val recordingGranted = ContextCompat.checkSelfPermission(
            activity!!,
            Manifest.permission.RECORD_AUDIO
        ) == PackageManager.PERMISSION_GRANTED
        if (!recordingGranted) {
            ActivityCompat.requestPermissions(
                activity,
                arrayOf(Manifest.permission.RECORD_AUDIO),
                1
            )
        } else {
            Log.i(javaClass.name, "Permission for recording audio already granted")
        }
    }
}