package com.sus.jnicmake;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import com.sus.jnicmake.jni.JniImage;
import com.sus.jnicmake.jni.JniManager;

import java.io.File;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {
    private String mFilePath;
    private ImageView mDisplayIv;

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
        Log.d(TAG, stringFromJNI());
        JniManager jniManager = new JniManager();
        Log.d(TAG, jniManager.askQuestion("大象的左耳朵像什么？"));

        findViewById(R.id.btn_path);
        findViewById(R.id.btn_display);
        mDisplayIv = (ImageView) findViewById(R.id.img_view);
    }

    public void onGetPath(View view) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {

                    InputStream in = getResources().getAssets()
                            .open("test.jpg");
                    Bitmap bitmap = BitmapFactory.decodeStream(in);
                    File dirFile = getExternalCacheDir();
                    if (!dirFile.exists()) {
                        dirFile.mkdirs();
                    }

                    String filename = System.currentTimeMillis() + ".jpg";
                    File jpegFile = new File(dirFile, filename);

                    mFilePath = jpegFile.getAbsolutePath();
                    boolean flag = false;
                    if (bitmap != null) {
                        flag = JniImage.compressBitmap(bitmap, bitmap.getWidth(), bitmap.getHeight(), mFilePath, 20);
                    } else {
                        //Toast.makeText(MainActivity.this, "file not found", Toast.LENGTH_SHORT).show();
                    }
                    //Toast.makeText(MainActivity.this, flag ? "success 文件大小" + (jpegFile.length()/1024)+"kb": "fail", Toast.LENGTH_SHORT).show();
                } catch (Exception e) {

                }
            }
        }).start();


    }

    public void onDisplay(View view) {
        if (TextUtils.isEmpty(mFilePath)) {
            Toast.makeText(MainActivity.this, "请先压缩图片", Toast.LENGTH_SHORT).show();
            return;
        }
        Bitmap bitmap = BitmapFactory.decodeFile(mFilePath);
        mDisplayIv.setImageBitmap(bitmap);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
