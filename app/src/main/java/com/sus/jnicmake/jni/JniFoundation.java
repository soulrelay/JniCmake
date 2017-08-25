package com.sus.jnicmake.jni;

/**
 * Created by SuS on 2017/8/24.
 */

public class JniFoundation {

    static {
        System.loadLibrary("native-all");
    }

    public native String stringFromJNI();

}
