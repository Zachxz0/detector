package com.zoson.detection.jni;

public class JniInterface
{

	public static native long newContext(String path);
	public static native long newDetector(long ptr,String path);
	public static native long getDetector(long ptr,int index); 

	public static native void initJni(long ptr);
	public static native void deInitJni(long ptr);
	public static native byte[] doDetectJni(long ptr,byte[] input,int w,int h,int c);
}