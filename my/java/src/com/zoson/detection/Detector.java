package com.zoson.detection;
import com.zoson.detection.proto.*;
import com.zoson.detection.jni.*;
public class Detector
{
	long _ptr = -1;
	Detector(long ptr)
	{
		_ptr = ptr;
		//System.out.println("detector init ptr is "+_ptr);
	}

	public int init()
	{
		if(_ptr!=-1)
		{
			JniInterface.initJni(_ptr);
			return 0;
		}else{
			System.out.println("Error:detector init ptr is null");
			return -1;
		}
	}

	public int deInit()
	{
		if(_ptr!=-1)
		{
			JniInterface.deInitJni(_ptr);
			return 0;
		}else{
			System.out.println("Error:detector init ptr is null");
			return -1;
		}
	}

	public ZosonProto.DetectOutput doDetect(byte[] in,int w,int h,int c )
	{
		if(_ptr!=-1)
		{
			byte[] outs=  JniInterface.doDetectJni(_ptr,in, w, h, c);
			try{
				ZosonProto.DetectOutput out = ZosonProto.DetectOutput.parseFrom(outs);
				return out;
			}catch(Exception e)
			{
				return null;
			}
		}else{
			System.out.println("Error:detector init ptr is null");
		}
		return null;
	}

	
}