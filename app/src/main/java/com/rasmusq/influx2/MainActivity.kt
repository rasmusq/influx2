package com.rasmusq.influx2

import android.graphics.Canvas
import android.graphics.Color
import android.os.Bundle
import android.util.Log
import android.view.MotionEvent
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.WindowInsetsControllerCompat
import com.rasmusq.influx2.databinding.ActivityMainBinding
import kotlin.random.Random

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private val mainHandler: MainHandler = MainHandler()
    private var screenHandler: ScreenHandler? = null
    private var audioHandler: AudioHandler? = null

    private fun onDraw(canvas: Canvas) {
        mainHandler.draw(canvas)
    }
    private fun onAudio(inputBuffer: ShortArray, outputBuffer: ShortArray) {
        mainHandler.audio(inputBuffer, outputBuffer)
    }

    private fun onSurfaceChanged(left: Int, top: Int, right: Int, bottom: Int) {
        // Override this
    }

    private fun onMotionEvent(motionEvent: MotionEvent) {
        Log.println(Log.VERBOSE, "MainActivity", motionEvent.toString())
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

//        binding = ActivityMainBinding.inflate(layoutInflater)
//        setContentView(binding.root)
        screenHandler = ScreenHandler(this, ::onDraw, ::onSurfaceChanged, ::onMotionEvent)
        setContentView(requireNotNull(screenHandler).surfaceView)

        audioHandler = AudioHandler(this, ::onAudio)
        audioHandler?.createAudioTrack()
        audioHandler?.startPlayingAudio()

        hideUI()

        // Example of a call to a native method
//        val result: Int = initAudioStream();
//        Log.println(Log.VERBOSE,"MainActivity", "Result of initAudioStream: $result");
//        activateAudio();
    }

    private fun hideUI() {
        val windowInsetsController =
            WindowCompat.getInsetsController(window, window.decorView)
        // Configure the behavior of the hidden system bars.
        windowInsetsController.systemBarsBehavior =
            WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
        windowInsetsController.hide(WindowInsetsCompat.Type.statusBars())
        windowInsetsController.hide(WindowInsetsCompat.Type.navigationBars())
    }

    /** EXAMPLES FOR POTENTIALLY IMPLEMENTING NATIVE AUDIO PROCESSING IN THE FUTURE
     * A native method that is implemented by the 'influx2' native library,
     * which is packaged with this application.
     */
    external fun initAudioStream(): Int
    external fun activateAudio(): String

    companion object {
        // Used to load the 'influx2' library on application startup.
        init {
            System.loadLibrary("influx2")
        }
    }
}