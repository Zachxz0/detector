package com.zoson.detection.jni;

public class DetectorJni
{
	public DetectorJni(){}

	public native void init();
	public native void deInit();
	public native void run();
	public native String test();
}
