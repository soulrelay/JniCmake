#include <jni.h>
#include <string>

jstring printHello(JNIEnv * env){
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
jstring
Java_com_sus_jnicmake_jni_JniFoundation_stringFromJNI(JNIEnv *env, jobject instance) {

    jstring hello = printHello(env);
    return hello;
}
