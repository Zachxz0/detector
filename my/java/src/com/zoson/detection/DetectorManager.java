package com.zoson.detection;

import com.zoson.detection.jni.*;

public class DetectorManager
{
	DetectorJni m_jni;
	public DetectorManager()
	{
		m_jni = new DetectorJni();
		System.out.println("DetectorManager");
	}

	public String test(){return m_jni.test();}
}