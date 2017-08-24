#include <jni.h>
#include <string>

#include <stdlib.h>

# ifdef Debug

#include <android/log.h>

#define LOG_TAG "来自native-log:"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
# endif

extern "C"
jstring
Java_com_sus_jnicmake_jni_JniManager_askQuestion(JNIEnv *env, jobject instance, jstring question_) {
    const char *question = env->GetStringUTFChars(question_, JNI_FALSE);
    char *answer = "右耳朵";

    char *data = (char *) malloc(strlen(question) + strlen(answer) + 1);
    strcpy(data, question);
    strcat(data, " : ");
    strcat(data, answer);
#ifdef Debug
    LOGE("我在Java_com_sus_jnicmake_jni_JniManager_askQuestion打印的log\n");
    LOGE("问题：%s",data);
# endif
    return env->NewStringUTF(data);

}


