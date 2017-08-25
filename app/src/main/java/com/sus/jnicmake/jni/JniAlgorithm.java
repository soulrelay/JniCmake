package com.sus.jnicmake.jni;

import android.graphics.Bitmap;

/**
 * Created by SuS on 2017/8/24.
 */

public class JniAlgorithm {


    static {
        System.loadLibrary("native-all");
    }

    public native String reverseSentence(String sentence);


}
