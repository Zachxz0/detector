package com.zoson.detection;
import com.zoson.detection.proto.*;
import cn.edu.scut.iais.proto.*;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.PrintStream;
import com.google.protobuf.TextFormat;
import java.awt.image.BufferedImage;
import cn.edu.scut.iais.*;
import java.io.File;
import javax.imageio.ImageIO;

public class MainForTest
{
	public static void main(String[] args)
	{
		System.loadLibrary("detectorJni");
		System.out.println("main for test");
		//DetectorContext ctx = new DetectorContext();
		//DetectorManager mger = new DetectorManager();
		//System.out.println(mger.test());
		System.out.println("current dir::"+System.getProperty("user.dir"));
		//test_proto();
		//test_dcontext();
		test_imageRecognition();
	}

	public static boolean test_imageRecognition()
	{
		ImageRecognition im = new ImageRecognition(System.getProperty("user.dir")+"/../config/dcontext.prototxt");
		//ImageRecognition im = new ImageRecognition(System.getProperty("user.dir")+"/../config/dcontext.prototxt");
		im.init();
		DetectorProto.ObjectDetect res =  im.detectObject("/home/zoson/detector/my/data/test/ly.jpg");
		System.out.println("object count::"+res.getObjectListCount());
		System.out.println("object state code ::"+res.getStateCode());
		System.out.println("object count::"+res.getObjectListCount());
		int count = res.getObjectListCount();
		for(int i=0;i<count;++i)
		{
			DetectorProto.Object ob = res.getObjectList(i);
			System.out.println("Object id "+ob.getObjectId());
			System.out.println("Object score "+ob.getScore());
			System.out.println("Object x "+ob.getX());
			System.out.println("Object y "+ob.getY());
			System.out.println("Object w "+ob.getW());
			System.out.println("Object H "+ob.getH());
		}
		DetectorProto.SceneDetect res1 =  im.detectScene("/home/zoson/detector/my/data/test/hu.jpg");
		System.out.println("object id::"+res1.getSceneId());
		System.out.println("object score:"+res1.getScore());
		return false;
	}

	public static boolean test_dcontext()
	{
		DContext dc = new DContext(System.getProperty("user.dir")+"/../config/dcontext.prototxt");
		Detector dt = dc.getDetector(0);
		dt.init();
		File file = new File("/home/zoson/detector/my/data/test/cat.jpg");
        BufferedImage bi = null;
        try {
            bi = ImageIO.read(file);
        } catch (Exception e) {
            e.printStackTrace();
        }
        int width = bi.getWidth();
        int height = bi.getHeight();
        int minx = bi.getMinX();
        int miny = bi.getMinY();
        byte[] bytes = new byte[(width-minx)*(height-miny)*3];
        int index = 0;
        for (int i = minx; i < width; i++) {
            for (int j = miny; j < height; j++) {
                int pixel = bi.getRGB(i, j); // 下面三行代码将一个数字转换为RGB数字
                bytes[index++] = (byte)((pixel & 0xff0000) >> 16);
                bytes[index++] = (byte)((pixel & 0xff00) >> 8);
                bytes[index++] = (byte)((pixel & 0xff));
            }
        }

		for(int i=0;i<100000;++i)
		{
			
			ZosonProto.DetectOutput out = dt.doDetect(bytes,width-minx,height-miny,3);
			System.out.println(out.getResults(0).getXmin()+"");

			int c = 1000000000;
			while(c>0)
			{
				c--;
			}
		}
		//dt.deInit();
		return true;
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