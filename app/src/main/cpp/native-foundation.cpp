#include <jni.h>
#include <string>

jstring printHello(JNIEnv *env) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
jstring
Java_com_sus_jnicmake_jni_JniFoundation_stringFromJNI(JNIEnv *env, jobject instance) {

    jstring hello = printHello(env);
    return hello;
}

//访问java对象中num属性，并对其作加法运算
extern "C"
jint
Java_com_sus_jnicmake_jni_JniFoundation_addNum(JNIEnv *env, jobject instance) {

    //1.获得实例对应的class类
    jclass jcls = env->GetObjectClass(instance);

    //2.通过class类找到对应的field id
    //num 为java类中变量名，I 为变量的类型int
    jfieldID fid = env->GetFieldID(jcls, "num", "I");

    //3.通过实例object获得对应的field
    jint jnum = env->GetIntField(instance, fid);
    //add
    jnum += 10;

    return jnum;

}

//访问java中静态变量name，并对其赋值
extern "C"
void
Java_com_sus_jnicmake_jni_JniFoundation_accessStaticField(JNIEnv *env, jobject instance) {

    jclass jcls = env->GetObjectClass(instance);

    //2.通过class类找到对应的静态field id
    //name 为java类中变量名，Ljava.lang.String; 为变量的类型String
    jfieldID fid = env->GetStaticFieldID(jcls, "name", "Ljava.lang.String;");

    //3.通过class获得对应的静态field
    jstring jstr = (jstring) env->GetStaticObjectField(jcls, fid);
    // 将jstring类型转换成字符指针
    const char *cstr = env->GetStringUTFChars(jstr, JNI_FALSE);
    //定义字符数组
    char text[30] = "hello ";
    //拼接字符
    strcat(text, cstr);
    jstring new_str = env->NewStringUTF(text);

    // 将jstring类型的变量 ， 设置到java
    env->SetStaticObjectField(jcls, fid, new_str);

}

//访问java中私有变量age
extern "C"
void
Java_com_sus_jnicmake_jni_JniFoundation_accessPrivateField(JNIEnv *env, jobject instance) {

    jclass jcls = env->GetObjectClass(instance);

    jfieldID jfid = env->GetFieldID(jcls, "age", "I");

    jint jage = env->GetIntField(instance, jfid);

    jage++;

    env->SetIntField(instance, jfid, jage);

}

//访问java中public/private方法
extern "C"
void
Java_com_sus_jnicmake_jni_JniFoundation_accessPublicMethod(JNIEnv *env, jobject instance) {
    //1.获得实例对应的class类
    jclass jcls = env->GetObjectClass(instance);

    //2.通过class类找到对应的method id
    //setSex 为java类中方法名，Ljava.lang.String; 为变量的类型String
    jmethodID jmid = env->GetMethodID(jcls,"setSex","(Ljava/lang/String;)V");
    //定义一个性别赋值给java中的方法
    char c[10] = "male";
    jstring jsex = env->NewStringUTF(c);
    //3.通过obj获得对应的method
    env->CallVoidMethod(instance,jmid,jsex);

}

//访问java中static方法
extern "C"
jint
Java_com_sus_jnicmake_jni_JniFoundation_accessStaticMethod(JNIEnv *env, jobject instance) {

    //1.获得实例对应的class类
    jclass jcls = env->GetObjectClass(instance);

    //2.通过class类找到对应的method id
    jmethodID jmid = env->GetStaticMethodID(jcls,"getHeight","()I");

    //3.静态方法通过class获得对应的method
    return env->CallStaticIntMethod(jcls,jmid);

}

//访问java中父类方法
extern "C"
jstring
Java_com_sus_jnicmake_jni_JniFoundation_accessSuperMethod(JNIEnv *env, jobject instance) {

    //1.通过反射获得父类的class类
    jclass jpcls = env->FindClass("com/sus/jnicmake/jni/SuperJniFoundation");
    if(jpcls == NULL){
        char c[10] = "error";
        return env->NewStringUTF(c);
    }
    //2.通过class类找到对应的method id
    jmethodID jmid = env->GetMethodID(jpcls,"hello","(Ljava/lang/String;)Ljava/lang/String;");
    char c[20] = "happy new year";
    jstring new_str = env->NewStringUTF(c);
    //3.静态方法通过class获得对应的method
    return (jstring)env->CallNonvirtualObjectMethod(instance,jpcls,jmid,new_str);
}

//传递int参数
extern "C"
jint Java_com_sus_jnicmake_jni_JniFoundation_intMethod( JNIEnv* env, jobject jobj,jint num){

    return num*num;
}

//传递布尔类型
extern "C"
jboolean Java_com_sus_jnicmake_jni_JniFoundation_booleanMethod(JNIEnv* env,jobject jobj,jboolean flag){

    return !flag;
}


//传递string类型
extern "C"
jstring Java_com_sus_jnicmake_jni_JniFoundation_stringMethod(JNIEnv* env,jobject jobj,jstring jstr){
    const char * str = env->GetStringUTFChars(jstr,0);
    char c[120] = "lily ";
    strcat(c,str);
    env->ReleaseStringUTFChars(jstr,str);
    return env->NewStringUTF(c);
}

extern "C"
jint  Java_com_sus_jnicmake_jni_JniFoundation_intArrayMethod(JNIEnv* env,jobject jobj,jintArray jarr){
    jint len=0,sum=0;
    len = env->GetArrayLength(jarr);
    jint* body = env->GetIntArrayElements(jarr,0);
    jint i=0;
    for(;i<len;i++){
        sum+=body[i];
    }
    return sum;
}


//传递person自定义对象给jni函数
extern "C"
jobject  Java_com_sus_jnicmake_jni_JniFoundation_objectMethod(JNIEnv* env,jobject jobj,jobject jperson){
    jclass jcls = env->GetObjectClass(jperson);
//    jclass jcls =  env->FindClass("com/sus/jnicmake/model/Person"); //或反射得Person类引用

    if(jcls == NULL){
        return env->NewStringUTF("not find class");
    }
    jmethodID constrocMID = env->GetMethodID(jcls,"<init>","(ILjava/lang/String;)V");
    if(constrocMID == NULL){
        return env->NewStringUTF("not find constroc method");
    }

    jstring str = env->NewStringUTF("honjane");

    jobject new_ojb = env->NewObject(jcls,constrocMID,21,str);  //构造一个对象，调用该类的构造函数，并且传递参数

    return new_ojb;
}


//传递ArrayList<Person>集合给jni函数
extern "C"
jobject  Java_com_sus_jnicmake_jni_JniFoundation_personArrayListMethod(JNIEnv* env,jobject jobj, jobject jlist){
    jclass jcls = env->GetObjectClass(jlist);
    if(jcls == NULL){
        return env->NewStringUTF("not find class");
    }
    jmethodID constrocMID = env->GetMethodID(jcls,"<init>","()V");
    if(constrocMID == NULL){
        return env->NewStringUTF("not find constroc method");
    }
    //创建一个Arraylist集合对象
    jobject list_obj = env->NewObject(jcls,constrocMID);
    //获取list的add方法id
    jmethodID list_add  = env->GetMethodID(jcls,"add","(Ljava/lang/Object;)Z");


    jclass jpersonCls = env->FindClass("com/sus/jnicmake/model/Person");

    jmethodID jpersonConstrocMID = env->GetMethodID(jpersonCls,"<init>","(ILjava/lang/String;)V");

    for(int i = 0 ; i < 3 ; i++)
    {
        jstring str = env->NewStringUTF("Native");
        //通过调用该对象的构造函数来new 一个 person实例
        jobject per_obj = env->NewObject(jpersonCls , jpersonConstrocMID , 20+i ,str);  //构造一个person对象
        //执行Arraylist类实例的add方法，添加一个person对象
        env->CallBooleanMethod(list_obj ,list_add, per_obj);
    }

    return list_obj;
}