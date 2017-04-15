package com.zoson.detection;

public class DetectorContext
{
	public DetectorContext()
	{
		//System.loadLibrary("Detector");
		// String lib_path = System.getProperty("java.library.path");
		// String curr_path = System.getProperty("user.dir");
		// String add_lib_path = lib_path+":"+curr_path+"/lib";
		// System.out.println(add_lib_path);
		// System.setProperty("java.library.path",add_lib_path);
		System.loadLibrary("DetectorJni");
	}

	public DetectorManager newManager()
	{
		return new DetectorManager();
	}
}