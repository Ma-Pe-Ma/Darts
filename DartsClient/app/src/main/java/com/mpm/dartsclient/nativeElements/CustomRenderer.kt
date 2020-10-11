package com.mpm.dartsclient.nativeElements

import android.content.res.AssetManager
import android.opengl.GLSurfaceView
import android.util.Log
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10


class CustomRenderer(var glSurface : GLSurfaceView) : GLSurfaceView.Renderer {

    init {
        System.loadLibrary("dartBoard")
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        surfaceCreated()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        surfaceChanged(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        drawFrame()
    }

    //native methods
    external fun init(assetManager: AssetManager?)
    private external fun surfaceCreated()
    private external fun surfaceChanged(w: Int, h: Int)
    private external fun drawFrame()
    external fun pause()
    external fun resume()
    external fun highLightSector(multiplier: Int, sector: Int)
    external fun setCurrentColorNative(red: Float, green: Float, blue: Float, alpha: Float)

    //helper methods
    fun setCurrentColor(color: Int) {
        val R : Float = (color shr 16 and 0xff) / 255.0f
        val G : Float= (color shr 8 and 0xff) / 255.0f
        val B : Float= (color and 0xff) / 255.0f

        setCurrentColorNative(R, G, B, 1.0f)
        glSurface.requestRender()
    }
}