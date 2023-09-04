package com.rasmusq.influx2

import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
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

    private var _androidTime: Long = 0
    private var _nativeTime: Long = 0
    private val _paint: Paint = Paint();
    private fun onDraw(canvas: Canvas) {
        mainHandler.draw(canvas)

        canvas.drawText("Android time: $_androidTime", 100.0f, 100.0f, _paint)
        canvas.drawText("Native time: $_nativeTime", 100.0f, 200.0f, _paint)
        _paint.setTextSize(55.0f)
    }
    private fun onAudio(inputBuffer: ShortArray, outputBuffer: ShortArray) {
        mainHandler.audio(inputBuffer, outputBuffer)
    }

    private fun onSurfaceChanged(left: Int, top: Int, right: Int, bottom: Int) {
        // Override this
    }

    private fun onMotionEvent(motionEvent: MotionEvent) {
        Log.println(Log.VERBOSE, "MainActivity", motionEvent.toString())
        if(motionEvent.action == MotionEvent.ACTION_DOWN)
            resumeAudio()
//            mainHandler.playingAudio = true
        else if(motionEvent.action == MotionEvent.ACTION_UP)
            pauseAudio()
//            mainHandler.playingAudio = false
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

//        binding = ActivityMainBinding.inflate(layoutInflater)
//        setContentView(binding.root)
        screenHandler = ScreenHandler(this, ::onDraw, ::onSurfaceChanged, ::onMotionEvent)
        setContentView(requireNotNull(screenHandler).surfaceView)

//        audioHandler = AudioHandler(this, ::onAudio)
//        audioHandler?.createAudioTrack()
//        audioHandler?.startPlayingAudio()

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
    external fun startAudioStream(): Int
    external fun stopAudioStream(): Int
    external fun closeAudioStream(): Int

    external fun pauseAudio(): Int

    external fun resumeAudio(): Int

    companion object {
        // Used to load the 'influx2' library on application startup.
        init {
            System.loadLibrary("influx2")
        }
    }
}