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

    private val mainHandler: MainHandler = MainHandler()

    private lateinit var _binding: ActivityMainBinding
    private lateinit var _screenHandler: ScreenHandler
    private lateinit var _audioHandler: AudioHandler

    private fun onDraw(canvas: Canvas) {
        mainHandler.draw(canvas)
    }

    private fun onSurfaceChanged(left: Int, top: Int, right: Int, bottom: Int) {

    }

    private fun onMotionEvent(motionEvent: MotionEvent) {
        Log.println(Log.VERBOSE, "MainActivity", motionEvent.toString())
        if (motionEvent.action == MotionEvent.ACTION_DOWN) {
            val y: Int = motionEvent.rawY.toInt();
            onMidi(intArrayOf(y, 2, 3, 4, 5));
        } else if(motionEvent.action == MotionEvent.ACTION_UP) {
            onMidi(intArrayOf(0, 2, 3, 4, 5));
            Log.i("MainActivity", "AudioNodeValueData: ${getAudioNodeValueData(0).get(0)}")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

//        binding = ActivityMainBinding.inflate(layoutInflater)
//        setContentView(binding.root)
        _screenHandler = ScreenHandler(this, ::onDraw, ::onSurfaceChanged, ::onMotionEvent)
        setContentView(requireNotNull(_screenHandler).surfaceView)

        _audioHandler = AudioHandler(this)

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
    external fun getAudioNodeValueData(audioNodeIndex: Int): IntArray;

//    external fun getAudioData(): IntArray;
//
//    external fun getOtherData(): IntArray;
//
//    external fun getPeakL(): Int;
//
//    external fun getPeakR(): Int;

    companion object {
        // Used to load the 'influx2' library on application startup.
        init {
            System.loadLibrary("influx2")
        }
    }
}