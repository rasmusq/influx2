package com.rasmusq.influx2

import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.Rect
import android.util.Log
import java.nio.IntBuffer
import kotlin.math.cos
import kotlin.math.sin
import kotlin.math.tan
import kotlin.random.Random

class MainHandler {

    private var _time: Double = 0.0
    private var _sampleCounter: Int = 0
    private val _paint: Paint = Paint();
    private val _sine: ShortArray = generateSine(256)

    fun generateSine(size: Int): ShortArray {
        var sine: ShortArray = ShortArray(size)
        for(i in sine.indices) {
            sine[i] = ((sin((i.toDouble() / sine.size.toDouble()) * 2 * 3.141593)) * Short.MAX_VALUE).toInt().toShort()
        }
        return sine
    }
    fun getSine(x: Short): Short {
        val position = x.toDouble() * _sine.size.toDouble() / Short.MAX_VALUE.toDouble()
//        Log.println(Log.VERBOSE, "MainHandler", "Hello ${position}")
        return _sine[position.toInt()];
    }

    fun draw(canvas: Canvas) {
        _time += 0.05;
        canvas.drawColor(Color.rgb(((Math.sin(_time)+1)*0.5*255).toInt(), ((Math.cos(_time)+1)*0.5*255).toInt(), ((Math.cos(_time/3)+1)*0.5*255).toInt()))
        //canvas.drawColor(Color.rgb(255, 255, 255))
        for(i in 1..128) {
            val x = i * 5 + 200
            val y = getSine(i.toShort()).toInt() + 200;
            canvas.drawRect(Rect(x, y, x + 5, y + 5), _paint)
        }
        _paint.setTextSize(55.0f)
    }

}