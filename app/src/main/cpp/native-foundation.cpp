#include <jni.h>
#include <string>
#include <malloc.h>

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

//调用c++printf函数
extern "C"
void Java_com_sus_jnicmake_jni_JniFoundation_printC( JNIEnv* env, jobject jobj){

    printf("JNI printf : hello JNI");
}

extern "C"
jstring Java_com_sus_jnicmake_jni_JniFoundation_handlerString( JNIEnv* env, jobject jobj,jstring jstr){
    //utf8
    const char* chr = env->GetStringUTFChars(jstr,0);
    //unicode
    const jchar* chr1 = env->GetStringChars(jstr,0);

    jsize jlen = env->GetStringUTFLength(jstr);
    jsize jlen1 = env->GetStringLength(jstr);

    char c[120] = "你好 2017";
    //把chr拼接到c
    strcat(c,chr);

    jstring new_str = env->NewStringUTF(c);

    env->ReleaseStringUTFChars(jstr,chr);
    env->ReleaseStringChars(jstr,chr1);

    return new_str;
}


char * jstringToChar(JNIEnv * env,jstring jstr){

    char* rtn = NULL;
    //等到String的class
    jclass jcls = env->FindClass("java/lang/String");
    //定义string的编码格式
    jstring strcode = env->NewStringUTF("utf-8");
    //获得string的getBytes method Id
    jmethodID jmid = env->GetMethodID(jcls, "getBytes", "(Ljava/lang/String;)[B");
    //调用getBytes方法 返回一个jbyteArray
    jbyteArray byteArr= (jbyteArray)env->CallObjectMethod(jstr, jmid, strcode);

    jsize alen = env->GetArrayLength(byteArr);
    //数组指向一个byte指针
    jbyte* pbyte = env->GetByteArrayElements(byteArr, 0);
    if (alen > 0)
    {
        //申请数组长度+1个内存空间
        rtn = (char*)malloc(alen + 1);
        //从源pbyte所指的内存地址的起始位置开始拷贝alen个字节到目标rtn所指的内存地址的起始位置中
        memcpy(rtn, pbyte, alen);
        rtn[alen] = 0;
    }
    //释放
    env->ReleaseByteArrayElements(byteArr, pbyte, 0);
    return rtn;
}


//string to byte
extern "C"
jbyteArray Java_com_sus_jnicmake_jni_JniFoundation_handlerStrToByte( JNIEnv* env, jobject jobj,jstring jstr){

    char * chr = NULL;
    chr =jstringToChar(env, jstr);

    jbyteArray RtnArr = NULL;  //下面一系列操作把chr转成jbyteArray 返回出去
    RtnArr = env->NewByteArray(strlen(chr));

    env->SetByteArrayRegion(RtnArr, 0, strlen(chr), (jbyte*)chr );

    //释放chr
    if(chr) {
        free(chr);
    }

    return RtnArr;
}

extern "C"
/**
*抛出异常的本地方法
*/
void
Java_com_sus_jnicmake_jni_JniFoundation_doit(
        JNIEnv* env,
        jobject jobj) {
    //std::string hello = "Hello from C++";
    jthrowable exc;
    jclass jclasz = env->GetObjectClass(jobj);

    jmethodID mid = env->GetMethodID(jclasz,"callback","()V");
    if(mid == NULL){
        return;
    }
    env->CallVoidMethod(jobj,mid);
    //确定是否某个异常正被抛出
    exc = env->ExceptionOccurred();
    if(exc){
        jclass newExc;

        //本地代码通过调用 ExceptionDescribe 来输出 一个关于这个异常的描述信息，
        env->ExceptionDescribe();
        //通过调用 ExceptionClear 清除异常信系
        env->ExceptionClear();
        newExc = env->FindClass("java/lang/IllegalArgumentException");
        if(newExc == NULL){
            return;
        }
        //JNI 抛出的异常(例如，通过 ThrowNew 方法)不 被处理的话，不会立即终止本地方法的执行
        env->ThrowNew(newExc,"this is c exception");
    }
}


/**
  异常工具类方法
  */
extern "C"
void Java_com_sus_jnicmake_jni_JniFoundation_JNUThrowByName(JNIEnv *env , jobject jobj,jstring  jname, jstring jmsg)
{
    const char * name = env->GetStringUTFChars(jname,0);
    const char * msg = env->GetStringUTFChars(jmsg,0);
    jclass clasz = env->FindClass(name);
    if(clasz != NULL){
        env->ThrowNew(clasz,msg);
    }
    env->DeleteLocalRef(clasz);
}