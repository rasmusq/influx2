package com.rasmusq.influx2

import android.graphics.Canvas
import android.os.Bundle
import android.util.Log
import android.view.MotionEvent
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.WindowInsetsControllerCompat
import com.rasmusq.influx2.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private val mainHandler: MainHandler = MainHandler()
    private var screenHandler: ScreenHandler? = null
    private var audioHandler: AudioHandler? = null

    private fun onDraw(canvas: Canvas) {
        mainHandler.draw(canvas)


    }

    private fun onSurfaceChanged(left: Int, top: Int, right: Int, bottom: Int) {

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

        audioHandler = AudioHandler(this)

        hideUI()

        // Example of a call to a native method
        initAudioStream();
    }

    override fun onStop() {
        super.onStop()
        stopAudioStream()
    }
    override fun onDestroy() {
        super.onDestroy()
        stopAudioStream()
        closeAudioStream()
    }

    override fun onResume() {
        super.onResume()
        startAudioStream()
    }

    private fun hideUI() {
        //NOTE: android:theme in the AndroidManifest.xml file also determines some UI elements
        val windowInsetsController =
            WindowCompat.getInsetsController(window, window.decorView)
        // Configure the behavior of the hidden system bars.
        windowInsetsController.systemBarsBehavior =
            WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
        windowInsetsController.hide(WindowInsetsCompat.Type.statusBars())
        windowInsetsController.hide(WindowInsetsCompat.Type.navigationBars())
    }

    external fun initAudioStream(): Int
    external fun startAudioStream(): Int
    external fun stopAudioStream(): Int
    external fun closeAudioStream(): Int
    external fun onMidi(midiData: IntArray): Int

    companion object {
        // Used to load the 'influx2' library on application startup.
        init {
            System.loadLibrary("influx2")
        }
    }
}