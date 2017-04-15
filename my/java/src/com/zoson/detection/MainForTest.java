package com.zoson.detection;

import com.zoson.detection.proto.*;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.PrintStream;
import com.google.protobuf.TextFormat;

public class MainForTest
{
	public static void main(String[] args)
	{
		System.out.println("main for test");
		DetectorContext ctx = new DetectorContext();
		DetectorManager mger = new DetectorManager();
		System.out.println(mger.test());
		System.out.println("current dir::"+System.getProperty("user.dir"));
		test_proto();
	}

	public static boolean test_proto()
	{
		try
		{
			//read proto buf
			String file = System.getProperty("user.dir")+"/../config/test.protobuf";
			System.out.println(file);
			FileInputStream fi = new FileInputStream(file);
			ZosonProto.TestProto test = ZosonProto.TestProto.parseFrom(fi);
			System.out.println(test.getClazzNum());
			fi.close();
			//read proto txt
			String file2 = System.getProperty("user.dir")+"/../config/test.prototxt";
			FileInputStream fi2 = new FileInputStream(file2);
			System.out.println(file2);
			byte[] buf = new byte[1024];
			int len;
			String in = new String();
			while((len=fi2.read(buf))!=-1){
				in += new String(buf);
			}
			in = in.split("\0")[0];
			System.out.println("in ::"+ in+" size:"+in.length());
			TextFormat.Parser txfp = TextFormat.getParser(); 
			ZosonProto.TestProto.Builder builder = ZosonProto.TestProto.newBuilder();
			txfp.merge(in,builder);
			System.out.println(builder.getClazzNum());
			fi2.close();

		}catch(FileNotFoundException e){
			System.out.println("file open error");
		}catch(IOException e){
			e.printStackTrace();
			System.out.println("parse proto error");
		}
		
		return true;
	}
}