package com.zoson.detection;

import com.zoson.detection.jni.*;

public class DContext
{
	long _ptr = -1;
	public DContext(String path)
	{
		//System.out.println("dcontext init path is "+path);
		_ptr = JniInterface.newContext(path);
		//System.out.println("dcontext init ptr is "+_ptr);
	}

	public Detector getDetector(int index)
	{
		long ptr = JniInterface.getDetector(_ptr,index);
		return new Detector(ptr);
	}
}