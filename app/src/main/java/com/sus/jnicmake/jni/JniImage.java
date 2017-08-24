package com.sus.jnicmake.jni;

import android.graphics.Bitmap;

/**
 * Created by SuS on 2017/8/24.
 */

public class JniImage {


    static {
        System.loadLibrary("native-all");
    }


    public static native boolean compressBitmap(Bitmap bitmap, int width, int height, String filePath,
                                                 int quality);
}
