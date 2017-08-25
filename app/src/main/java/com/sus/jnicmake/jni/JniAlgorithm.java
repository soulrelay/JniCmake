package com.sus.jnicmake.jni;

/**
 * Created by SuS on 2017/8/24.
 */

public class JniAlgorithm {


    static {
        System.loadLibrary("native-all");
    }


    // 面试题58（一）：翻转单词顺序
    // 题目：输入一个英文句子，翻转句子中单词的顺序，但单词内字符的顺序不变。
    // 为简单起见，标点符号和普通字母一样处理。例如输入字符串"I am a student."，
    // 则输出"student. a am I"。
    public native String reverseSentence(String sentence);


}
