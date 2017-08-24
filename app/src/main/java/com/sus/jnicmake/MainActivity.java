package com.sus.jnicmake;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.sus.jnicmake.jni.JniManager;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = MainActivity.class.getCanonicalName();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-all");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.tv_hello);
        tv.setText(stringFromJNI());

        Log.d(TAG,stringFromJNI());
        JniManager jniManager = new JniManager();
        Log.d(TAG,jniManager.askQuestion("大象的左耳朵像什么？"));
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
