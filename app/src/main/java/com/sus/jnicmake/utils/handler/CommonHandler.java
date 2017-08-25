package com.sus.jnicmake.utils.handler;

import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.util.Log;

import java.lang.ref.WeakReference;

/**
 * 通用的Handler
 * Created by SuS
 * Date: 17/8/25
 */

public class CommonHandler<T extends IHandlerMessage> extends Handler {
    private static final String TAG = CommonHandler.class.getSimpleName();

    private WeakReference<T> reference;
    private String className;

    public CommonHandler(T t) {
        className = t.getClass().getSimpleName();
        reference = new WeakReference<T>(t);
    }

    @Override
    public void handleMessage(Message msg) {
        super.handleMessage(msg);
        if (reference == null) {
            Log.i(TAG, "reference is null, class : " + className);
            return;
        }
        T t = reference.get();
        if (t == null) {
            Log.i(TAG, "T null, class : " + className);
            return;
        }
        if (t instanceof Fragment) {
            Fragment fragment = (Fragment) t;
            if (fragment.getActivity() == null || !(fragment).isAdded()) {
                Log.i(TAG, "getActivity() is null or is not add, class : " + className);
                return;
            }
        }
        t.handlerCallback(msg);

    }

    @Override
    protected void finalize() throws Throwable {
        Log.i(TAG, "<<<<<     finalize      >>>>>    " + className + "   Handler :" + Integer.toHexString(hashCode()));
        super.finalize();
    }
}
