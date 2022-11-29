// package com.thkoeln.jmoeller.vins_mobile_androidport;

// import android.content.Context;
// import android.graphics.PixelFormat;
// import android.opengl.GLSurfaceView;
// import android.util.AttributeSet;
// import android.util.Log;
// import android.view.KeyEvent;
// import android.view.MotionEvent;
// import android.view.View;

// import javax.microedition.khronos.egl.EGL10;
// import javax.microedition.khronos.egl.EGLConfig;
// import javax.microedition.khronos.egl.EGLContext;
// import javax.microedition.khronos.egl.EGLDisplay;
// import javax.microedition.khronos.opengles.GL10;

// class VisualizeView extends GLSurfaceView {
//     private static final String TAG = "VisualizeJNI";
//     private static final boolean DEBUG = true;

//     public VisualizeView(Context context) {
//         super(context);
//         // Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
//         // supporting OpenGL ES 2.0 or later backwards-compatible versions.
//         setEGLConfigChooser(8, 8, 8, 0, 16, 0);
//         setEGLContextClientVersion(3);
//         setRenderer(new Renderer());
//     }

//     private float prevX;
//     private float prevY;
//     @Override
//     public boolean onTouchEvent(MotionEvent event) {
//         float x = event.getX();
//         float y = event.getY();

//         switch (event.getAction()) {
//             case MotionEvent.ACTION_MOVE: {
//                 float dx = x - prevX;
//                 float dy = y - prevY;
// //                Log.println(Log.WARN, "[INFO LOG]", "x : " + Float.toString(dx) + ", y : " + Float.toString(dy));
//                 VisualizeLib.cameraMove(dx, dy);
//             }
//         }
//         prevX = x;
//         prevY = y;
//         return true;
//     }

//     private static class Renderer implements GLSurfaceView.Renderer {
//         public void onDrawFrame(GL10 gl) {
//             VisualizeLib.render();
//         }

//         public void onSurfaceChanged(GL10 gl, int width, int height) {
//             VisualizeLib.resize(width, height);
//         }

//         public void onSurfaceCreated(GL10 gl, EGLConfig config) {
//             Log.println(Log.INFO, "[INFO LOG]", "onSurfaceCreated");
//             VisualizeLib.init();
//         }
//     }
// }

package com.thkoeln.jmoeller.vins_mobile_androidport;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

class VisualizeView extends GLSurfaceView {
    private static final String TAG = "VisualizeJNI";
    private static final boolean DEBUG = true;

    public VisualizeView(Context context) {
        super(context);
        // Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
        // supporting OpenGL ES 2.0 or later backwards-compatible versions.
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);
        setRenderer(new Renderer());
    }

    private float prevX;
    private float prevY;
    private double prevDistance;
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        float x = event.getX();
        float y = event.getY();

        switch (event.getAction() & MotionEvent.ACTION_MASK) {
            case MotionEvent.ACTION_DOWN:
                prevX = x;
                prevY = y;
                break;
            case MotionEvent.ACTION_POINTER_DOWN:
                if (event.getPointerCount() == 2) {
                    float nowDistanceX = x - event.getX(1);
                    float nowDistanceY = y - event.getY(1);
                    prevDistance = Math.sqrt(nowDistanceX * nowDistanceX + nowDistanceY * nowDistanceY);
                }
                break;
            case MotionEvent.ACTION_MOVE:
                if (event.getPointerCount() == 1) {
                    float dx = x - prevX;
                    float dy = y - prevY;
//                Log.println(Log.WARN, "[INFO LOG]", "x : " + Float.toString(dx) + ", y : " + Float.toString(dy));

                    if(dx*dx + dy*dy < 400)
                    VinsJNI.visualizeCameraMove(dx, dy);
                    prevX = x;
                    prevY = y;
                } else if (event.getPointerCount() == 2) {
                    float nowDistanceX = x - event.getX(1);
                    float nowDistanceY = y - event.getY(1);
                    double nowDistance = Math.sqrt(nowDistanceX*nowDistanceX + nowDistanceY*nowDistanceY);

                    VinsJNI.visualizeCameraDistance((nowDistance - prevDistance)*0.05);
                    prevDistance = nowDistance;
                }
                break;
            case MotionEvent.ACTION_UP:
                break;
        }
        return true;
    }

    private static class Renderer implements GLSurfaceView.Renderer {
        public void onDrawFrame(GL10 gl) {
            VinsJNI.visualizeRender();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            VinsJNI.visualizeResize(width, height);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            Log.println(Log.INFO, "[INFO LOG]", "onSurfaceCreated");
            VinsJNI.visualizeInit();
        }
    }
}
