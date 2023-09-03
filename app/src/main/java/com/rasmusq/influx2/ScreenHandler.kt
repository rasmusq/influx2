package com.rasmusq.influx2

import android.app.Activity
import android.content.Context
import android.graphics.Canvas
import android.graphics.PixelFormat
import android.view.MotionEvent
import android.view.SurfaceHolder
import android.view.SurfaceView

class ScreenHandler(
    activity: Activity,
    onDraw: (canvas: Canvas) -> Unit,
    onSurfaceChanged: (left: Int, top: Int, right: Int, bottom: Int) -> Unit,
    onMotionEvent: (motionEvent: MotionEvent) -> Unit
) {

    private val _surfaceView: SurfaceView = createSurfaceView(activity)
    val surfaceView: SurfaceView
        get() = _surfaceView
    private val _surfaceHolderCallback: SurfaceHolder.Callback = createSurfaceHolderCallback()
    private val _surfaceHolder: SurfaceHolder =
        createSurfaceHolder(_surfaceView, _surfaceHolderCallback)
    private val _drawThread: Thread = createThread()
    private val _drawLoopIsRunning = true
    private val _drawFunc = onDraw;
    private val _surfaceChangeFunc = onSurfaceChanged;
    private val _motionEventFunc = onMotionEvent;

    init {
        startThread()
    }

    private fun createSurfaceView(context: Context): SurfaceView {
        return object : SurfaceView(context) {
            override fun onTouchEvent(motionEvent: MotionEvent): Boolean {
                _motionEventFunc(motionEvent)
                return true
            }
        }
    }

    private fun createSurfaceHolderCallback(): SurfaceHolder.Callback {
        return object : SurfaceHolder.Callback {
            override fun surfaceCreated(surfaceHolder: SurfaceHolder) {}
            override fun surfaceChanged(
                surfaceHolder: SurfaceHolder,
                format: Int,
                width: Int,
                height: Int
            ) {
                _surfaceChangeFunc(0, 0, width, height)
            }

            override fun surfaceDestroyed(surfaceHolder: SurfaceHolder) {}
        }
    }

    private fun createSurfaceHolder(
        surfaceView: SurfaceView,
        surfaceHolderCallback: SurfaceHolder.Callback
    ): SurfaceHolder {
        val surfaceHolder = surfaceView.getHolder()
        surfaceHolder.setFormat(PixelFormat.TRANSPARENT)
        surfaceHolder.addCallback(surfaceHolderCallback)
        return surfaceHolder
    }

    private fun createThread(): Thread {
        return Thread {
            while (_drawLoopIsRunning) {
                draw()
            }
        }
    }

    private fun startThread() {
        _drawThread.start()
    }

    fun draw() {
        var canvas: Canvas? = null
        try {
            if (_surfaceHolder!!.surface.isValid) {
                canvas = _surfaceHolder!!.lockCanvas()
            }
            if (canvas != null) {
                _drawFunc(canvas)
            }
        } finally {
            if (canvas != null) {
                _surfaceHolder!!.unlockCanvasAndPost(canvas)
            }
        }
    }
}