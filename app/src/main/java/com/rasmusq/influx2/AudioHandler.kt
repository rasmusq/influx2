package com.rasmusq.influx2

import android.Manifest
import android.app.Activity
import android.content.Context
import android.content.pm.PackageManager
import android.util.Log
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat

class AudioHandler(activity: Activity) {

    init {
        checkAudioFeatures(activity)
        requestPermissions(activity)
    }

    fun checkAudioFeatures(context: Context) {
        val hasLowLatencyFeature =
            context.packageManager.hasSystemFeature(PackageManager.FEATURE_AUDIO_LOW_LATENCY)
        val hasProFeature =
            context.packageManager.hasSystemFeature(PackageManager.FEATURE_AUDIO_PRO)
        Log.println(Log.VERBOSE, "AudioHandler", "Low latency feature: $hasLowLatencyFeature")
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