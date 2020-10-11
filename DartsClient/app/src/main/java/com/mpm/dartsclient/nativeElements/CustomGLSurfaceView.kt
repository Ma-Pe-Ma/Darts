package com.mpm.dartsclient.nativeElements

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.MotionEvent

class CustomGLSurfaceView(context: Context?, attributes: AttributeSet? = null) : GLSurfaceView(context, attributes) {
    var customRenderer : CustomRenderer? = null

    init {
        System.loadLibrary("dartBoard")
        setEGLContextClientVersion(3);
        customRenderer = CustomRenderer(this)
        setRenderer(customRenderer)
        renderMode = RENDERMODE_WHEN_DIRTY
    }

    override fun onPause() {
        super.onPause()
        customRenderer?.pause()
    }

    override fun onResume() {
        super.onResume()
        customRenderer?.pause()
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        if (event.action == MotionEvent.ACTION_DOWN) {
            //nativeTogglePauseResume()
        }
        return true
    }
}
