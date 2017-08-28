package com.sus.jnicmake.jni;

import com.sus.jnicmake.model.Person;

import java.util.ArrayList;

/**
 * Created by SuS on 2017/8/24.
 */

public class JniFoundation extends SuperJniFoundation {

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

    //method --------
    private String sex = "female";

    private void setSex(String sex){
        this.sex = sex;
    }

    public String getSex(){
        return sex;
    }

    public native void accessPublicMethod();

    private static int height = 160;

    public static int getHeight(){
        return height;
    }

    public native int accessStaticMethod();

    public native String accessSuperMethod();

    //传参数
    public native int intMethod(int n);

    public native boolean booleanMethod(boolean bool);

    public native String stringMethod(String text);

    public native int intArrayMethod(int[] intArray);

    public native Person objectMethod(Person person);

    public native ArrayList<Person> personArrayListMethod(ArrayList<Person> persons);

}
