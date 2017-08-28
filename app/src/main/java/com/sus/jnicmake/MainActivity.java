package com.sus.jnicmake;

import android.content.res.AssetFileDescriptor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import com.sus.jnicmake.jni.JniAlgorithm;
import com.sus.jnicmake.jni.JniFoundation;
import com.sus.jnicmake.jni.JniImage;
import com.sus.jnicmake.jni.JniManager;
import com.sus.jnicmake.model.Person;
import com.sus.jnicmake.utils.handler.CommonHandler;
import com.sus.jnicmake.utils.handler.IHandlerMessage;
import com.wang.avi.AVLoadingIndicatorView;

import java.io.File;
import java.io.InputStream;
import java.text.DecimalFormat;
import java.util.ArrayList;

import static com.sus.jnicmake.MainActivity.HandlerMsg.MSG_COMPRESS_SUCCESS;

/**
 * Created by SuS
 * Date: 17/8/24
 */
public class MainActivity extends AppCompatActivity implements IHandlerMessage {
    private String mFilePath;
    private ImageView ivCompress;
    private CommonHandler<MainActivity> handler;
    private AVLoadingIndicatorView avLoadingIndicatorView;

    private static final String TAG = MainActivity.class.getCanonicalName();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-all");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example 1
        Log.d(TAG, stringFromJNI());
        // Example 2
        JniManager jniManager = new JniManager();
        Log.d(TAG, jniManager.askQuestion("大象的左耳朵像什么？"));

        initAlgorithm();

        intFoundation();

        initJpeg();
    }

    private void initJpeg() {
        // Example jpeg
        findViewById(R.id.btn_compress_display);
        ivCompress = (ImageView) findViewById(R.id.ivCompress);
        handler = new CommonHandler<>(this);
        avLoadingIndicatorView = (AVLoadingIndicatorView) findViewById(R.id.avi);
    }

    private void initAlgorithm() {
        JniAlgorithm jniAlgorithm = new JniAlgorithm();
        Log.d(TAG,jniAlgorithm.reverseSentence("I am a student."));
    }

    private void intFoundation() {
        JniFoundation jniFoundation = new JniFoundation();
        Log.d("jF stringFromJNI",jniFoundation.stringFromJNI());
        Log.d("jF addNum",jniFoundation.addNum()+"");

        jniFoundation.accessStaticField();
        Log.d("jF accessStaticField", jniFoundation.name);

        Log.d("main----age修改前",jniFoundation.getAge()+"");
        jniFoundation.accessPrivateField();
        Log.d("main----age修改后",jniFoundation.getAge()+"");

        //method
        Log.d("main----sex赋值前",jniFoundation.getSex());
        jniFoundation.accessPublicMethod();
        Log.d("main----sex赋值后",jniFoundation.getSex());


        Log.d("main----height调用java方法", jniFoundation.getHeight()+"");

        Log.d("main----height调用JNI方法",jniFoundation.accessStaticMethod()+"");
        Log.d("main----调用java super方法",jniFoundation.accessSuperMethod());

        //传参数
        Log.i("main----intMethod",jniFoundation.intMethod(4)+"");
        Log.i("main----booleanMethod",jniFoundation.booleanMethod(true)+"");
        Log.i("main----stringMethod",jniFoundation.stringMethod("hello"));
        Log.i("main----intArrayMethod",jniFoundation.intArrayMethod(new int[]{3,5,6,12,46,33})+"");
        Log.i("main----objectMethod",jniFoundation.objectMethod(new Person()).toString()+"");


        ArrayList<Person> personList = new ArrayList<>();
        Person person;
        for (int i =0 ;i<3;i++){
            person = new Person();
            person.setName("lily");
            person.setAge(10+i);
            personList.add(person);
        }
        Log.i("main----输出java list",personList.toString());
        Log.i("main----输出jni list",jniFoundation.personArrayListMethod(personList).toString());


        //字符串处理

        Log.i("main----handlerString",jniFoundation.handlerString(" happy new year"));
        byte [] bytes = jniFoundation.handlerStrToByte("honjane");
        for (byte b : bytes) {
            Log.i("main---handlerStrToByte", (char)b+"");
        }

        //异常
        try {
//            jniFoundation.doit();
            jniFoundation.JNUThrowByName("callback", "this is c NoSuchElementException");
        } catch (Exception e) {
            Log.e("main---", "java in " + e.toString());
        }


    }

    public void onCompressDisplay(View view) {
        avLoadingIndicatorView.show();
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {

                    AssetFileDescriptor fd = getAssets().openFd("test.png");
                    long size = fd.getLength();
                    InputStream in = getResources().getAssets()
                            .open("test.png");
                    Bitmap bitmap = BitmapFactory.decodeStream(in);
                    File dirFile = getExternalCacheDir();
                    if (!dirFile.exists()) {
                        dirFile.mkdirs();
                    }

                    String filename = System.currentTimeMillis() + ".jpg";
                    File jpegFile = new File(dirFile, filename);

                    mFilePath = jpegFile.getAbsolutePath();
                    boolean flag;
                    if (bitmap != null) {
                        flag = JniImage.compressBitmap(bitmap, bitmap.getWidth(), bitmap.getHeight(), mFilePath, 20);
                        if (flag) {
                            Message msg = new Message();
                            msg.arg1 = (int) (jpegFile.length() / 1024);
                            msg.arg2 = (int) (size);
                            msg.what = HandlerMsg.MSG_COMPRESS_SUCCESS;
                            handler.sendMessage(msg);
                        } else {
                            handler.sendEmptyMessage(HandlerMsg.MSG_COMPRESS_FAIL);
                        }
                    } else {
                        handler.sendEmptyMessage(HandlerMsg.MSG_FILE_NOT_FOUND);
                    }
                } catch (Exception e) {

                }
            }
        }).start();


    }

    @Override
    public void handlerCallback(Message msg) {
        switch (msg.what) {
            case MSG_COMPRESS_SUCCESS:
                double arg2 = msg.arg2;
                double size = arg2/1024/1024;
                Toast.makeText(MainActivity.this, "原始图片大小为" + new DecimalFormat("######0.0").format(size) + "MB"
                        + "图片压缩成功压缩至" + msg.arg1 + "kb", Toast.LENGTH_LONG).show();
                Bitmap bitmap = BitmapFactory.decodeFile(mFilePath);
                ivCompress.setImageBitmap(bitmap);
                break;
            case HandlerMsg.MSG_COMPRESS_FAIL:
                Toast.makeText(MainActivity.this, "图片压缩失败", Toast.LENGTH_LONG).show();
                break;
            case HandlerMsg.MSG_FILE_NOT_FOUND:
                Toast.makeText(MainActivity.this, "待压缩文件不存在", Toast.LENGTH_LONG).show();
                break;
            default:
                break;
        }
        avLoadingIndicatorView.hide();
    }


    public interface HandlerMsg {

        int MSG_COMPRESS_SUCCESS = 2001;//图片压缩成功

        int MSG_COMPRESS_FAIL = 2002;//图片压缩失败

        int MSG_FILE_NOT_FOUND = 2003;//未找到指定文件
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
