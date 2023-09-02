package com.rasmusq.influx2

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.TextView
import com.rasmusq.influx2.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method

        val result: Int = initAudioStream();
        Log.d("MainActivity", "Result of initAudioStream: $result");
        activateAudio();
    }

    /**
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