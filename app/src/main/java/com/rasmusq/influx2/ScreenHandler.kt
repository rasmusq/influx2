package com.rasmusq.influx2

import android.app.Activity
import android.content.Context
import android.graphics.Canvas
import android.graphics.PixelFormat
import android.util.DisplayMetrics
import android.view.MotionEvent
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.View

abstract class ScreenHandler(activity: Activity) {

    private var surfaceView: SurfaceView? = null
    private var surfaceHolder: SurfaceHolder? = null
    private var surfaceHolderCallback: SurfaceHolder.Callback? = null
    private var drawThread: Thread? = null
    private val drawLoopIsRunning = true

    init {
        surfaceHolder = initializeSurface(activity)
        initializeThread()
        startThread()
    }

    abstract fun OnDraw(canvas: Canvas)
    abstract fun OnSurfaceChanged(left: Int, top: Int, right: Int, bottom: Int)
    abstract fun OnMotionEvent(motionEvent: MotionEvent)

    private fun createSurfaceView(context: Context): SurfaceView {
        var surfaceView: SurfaceView = object : SurfaceView(context) {
            override fun onTouchEvent(motionEvent: MotionEvent): Boolean {
                OnMotionEvent(motionEvent)
                return true
            }
        }
        return surfaceView;
    }
    private fun createSurfaceHolder(context: Context): SurfaceHolder {

        surfaceHolderCallback = object : SurfaceHolder.Callback {
            override fun surfaceCreated(surfaceHolder: SurfaceHolder) {}
            override fun surfaceChanged(
                surfaceHolder: SurfaceHolder,
                format: Int,
                width: Int,
                height: Int
            ) {
                OnSurfaceChanged(0, 0, width, height)
            }

            override fun surfaceDestroyed(surfaceHolder: SurfaceHolder) {}
        }
        surfaceHolder = surfaceView.getHolder()
        surfaceHolder.setFormat(PixelFormat.TRANSPARENT)
        surfaceHolder.addCallback(surfaceHolderCallback)
        return surfaceHolder
    }

    private fun initializeThread() {
        drawThread = Thread {
            while (drawLoopIsRunning) {
                draw()
            }
        }
        drawThread!!.name = "Draw Thread"
    }

    private fun startThread() {
        drawThread!!.start()
    }

    fun draw() {
        var canvas: Canvas? = null
        try {
            if (surfaceHolder!!.surface.isValid) {
                canvas = surfaceHolder!!.lockCanvas()
            }
            if (canvas != null) {
                OnDraw(canvas)
            }
        } finally {
            if (canvas != null) {
                surfaceHolder!!.unlockCanvasAndPost(canvas)
            }
        }
    }
}