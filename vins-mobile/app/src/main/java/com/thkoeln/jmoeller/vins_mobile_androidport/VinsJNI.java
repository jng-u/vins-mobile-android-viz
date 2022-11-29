package com.thkoeln.jmoeller.vins_mobile_androidport;

import android.view.Surface;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.Serializable;
import java.nio.ByteBuffer;

/**
 * JNI Java Part
 */
public class VinsJNI implements Serializable {

    // Used to load the 'native-lib' library on application startup.
    static { System.loadLibrary("NativeLib"); }
    
    public native void init();
    
    public static native void onImageAvailable(int width, int height, int rowStrideY, ByteBuffer bufferY, 
                                               int rowStrideUV, ByteBuffer bufferU, ByteBuffer bufferV, 
                                               Surface surface, long timeStamp, boolean isScreenRotated,
                                               float virtualCamDistance);
    
    public static native void updateViewInfo(TextView tvX, TextView tvY, TextView tvZ,
                                             TextView tvTotal, TextView tvLoop, TextView tvFeature,
                                             TextView tvBuf, ImageView initImage);
    
    public static native void onPause();
    
    public static native void onARSwitch(boolean isChecked);
    public static native void onLoopSwitch(boolean isChecked);

     public static native void visualizeInit();
     public static native void visualizeResize(int width, int height);
     public static native void visualizeRender();
     public static native void visualizeCameraMove(float dx, float dy);
     public static native void visualizeCameraDistance(double val);
}
