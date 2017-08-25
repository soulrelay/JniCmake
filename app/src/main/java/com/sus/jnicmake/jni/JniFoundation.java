package com.sus.jnicmake.jni;

/**
 * Created by SuS on 2017/8/24.
 */

public class JniFoundation {

    static {
        System.loadLibrary("native-all");
    }

    public native String stringFromJNI();

    public int num = 10;

    public native int addNum();

    //field ----
    public static String name = "SuS";

    public native void accessStaticField();

    private int age = 21;

    public int getAge(){
        return age;
    }

    public native void accessPrivateField();

}
