package com.rasmusq.influx2

import android.content.pm.ActivityInfo
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.Window
import android.view.WindowManager
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.WindowInsetsControllerCompat
import com.rasmusq.influx2.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private var mainHandler: MainHandler = MainHandler()
    private var screenHandler: ScreenHandler = ScreenHandler(this) {

    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

//        binding = ActivityMainBinding.inflate(layoutInflater)
//        setContentView(binding.root)
        setContentView(screenHandler.sur)

        hideUI()

        // Example of a call to a native method
//      val result: Int = initAudioStream();
//      Log.d("MainActivity", "Result of initAudioStream: $result");
//      activateAudio();
    }

    private fun hideUI() {
        val windowInsetsController =
            WindowCompat.getInsetsController(window, window.decorView)
        // Configure the behavior of the hidden system bars.
        windowInsetsController.systemBarsBehavior =
            WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
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