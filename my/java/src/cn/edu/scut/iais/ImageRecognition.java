package cn.edu.scut.iais;
import com.zoson.detection.*;
import cn.edu.scut.iais.proto.*;
import com.zoson.detection.proto.*;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.PrintStream;
import com.google.protobuf.TextFormat;
import java.io.File;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
public class ImageRecognition
{

	private DContext _ctx;
	private Detector _dtor;
	private Detector _claer;
	public ImageRecognition(String path)
	{
		_ctx = new DContext(path);
		_dtor = _ctx.getDetector(0);
		_claer = _ctx.getDetector(1);
	}

	public int init()
	{
		int ret =_dtor.init();
		if(ret!=0)System.out.println("Detector init failed");
		int ret1 = _claer.init();
		if(ret1!=0)System.out.println("Classifier init failed");
		return ret*ret1;

	}

	public int deInit()
	{
		return _dtor.deInit()*_claer.deInit();
	}

	public Bitmap decodeToRGB(String imgPath,DetectorProto.ObjectDetect.Builder ret)
	{
		File file = new File(imgPath);
		if(!file.exists())
		{
			ret.setStateCode(103);
			return null;
		}
        BufferedImage bi = null;
        try {
            bi = ImageIO.read(file);
        } catch (Exception e) {
        	e.printStackTrace();
        	ret.setStateCode(104);
			return null;
        }
        int width = bi.getWidth();
        int height = bi.getHeight();
        int minx = bi.getMinX();
        int miny = bi.getMinY();
        byte[] bytes = new byte[(width-minx)*(height-miny)*3];
        int index = 0;
    	for (int j = miny; j < height; j++) {
        	for (int i = minx; i < width; i++) {
                int pixel = bi.getRGB(i, j); 
                bytes[index++] = (byte)((pixel & 0xff0000) >> 16);
                bytes[index++] = (byte)((pixel & 0xff00) >> 8);
                bytes[index++] = (byte)((pixel & 0xff));
            }
        }
        Bitmap map = new Bitmap();
        map.w = width - minx;
        map.h = height - miny;
        map.data = bytes;
        return map;
	}

	public Bitmap decodeToRGB(String imgPath,DetectorProto.SceneDetect.Builder ret)
	{
		File file = new File(imgPath);
		if(!file.exists())
		{
			ret.setStateCode(103);
			return null;
		}
        BufferedImage bi = null;
        try {
            bi = ImageIO.read(file);
        } catch (Exception e) {
        	e.printStackTrace();
        	ret.setStateCode(104);
			return null;
        }
        int width = bi.getWidth();
        int height = bi.getHeight();
        int minx = bi.getMinX();
        int miny = bi.getMinY();
        byte[] bytes = new byte[(width-minx)*(height-miny)*3];
        int index = 0;
        for (int j = miny; j < height; j++) {
        	for (int i = minx; i < width; i++) {
                int pixel = bi.getRGB(i, j); 
                bytes[index++] = (byte)((pixel & 0xff0000) >> 16);
                bytes[index++] = (byte)((pixel & 0xff00) >> 8);
                bytes[index++] = (byte)((pixel & 0xff));
            }
        }
        Bitmap map = new Bitmap();
        map.w = width - minx;
        map.h = height - miny;
        map.data = bytes;
        return map;
	}

	public DetectorProto.ObjectDetect detectObject(String imgPath)
	{
		DetectorProto.ObjectDetect.Builder ret = DetectorProto.ObjectDetect.newBuilder();
		Bitmap map = decodeToRGB(imgPath,ret);
		if(map==null)return ret.build();
		return detectObject(map.data,map.w,map.h);
	}

	//make sure the format of bytes is rgb .
	public DetectorProto.ObjectDetect detectObject(byte[] bytes,int w,int h)
	{
		DetectorProto.ObjectDetect.Builder ret = DetectorProto.ObjectDetect.newBuilder();
		if(w<0||h<0||bytes.length!=w*h*3)
		{
			ret.setStateCode(102);
			return ret.build();
		}
		ZosonProto.DetectOutput out = _dtor.doDetect(bytes,w,h,3);
        int object_count = out.getResultsCount();
        for(int i=0;i<object_count;++i)
        {
        	ZosonProto.Result res = out.getResults(i);
        	DetectorProto.Object.Builder ob = DetectorProto.Object.newBuilder();
        	ob.setScore(res.getScore());
        	ob.setX((int)res.getXmin());
        	ob.setY((int)res.getYmin());
        	ob.setW((int)(res.getXmax()-res.getXmin()));
        	ob.setH((int)(res.getYmax()-res.getYmin()));
        	ob.setObjectId(res.getClazz());
        	ret.addObjectList(ob);
        }
        ret.setStateCode(100);
        ret.setObjectCount(object_count);
		return ret.build();
	}

	public DetectorProto.SceneDetect detectScene(String imgPath)
	{
		DetectorProto.SceneDetect.Builder ret = DetectorProto.SceneDetect.newBuilder();
		Bitmap map = decodeToRGB(imgPath,ret);
		if(map==null)return ret.build();
		return detectScene(map.data,map.w,map.h);
	}

	public DetectorProto.SceneDetect detectScene(byte[] bytes,int w ,int h)
	{
		DetectorProto.SceneDetect.Builder ret = DetectorProto.SceneDetect.newBuilder();
		if(w<0||h<0||bytes.length!=w*h*3)
		{
			ret.setStateCode(102);
			return ret.build();
		}
		ZosonProto.DetectOutput out = _claer.doDetect(bytes,w,h,3);
        int object_count = out.getResultsCount();
        ZosonProto.Result res = out.getResults(0);
        ret.setSceneId(res.getClazz());
        ret.setScore(res.getScore());
        ret.setStateCode(100);
        //sret.setObjectCount(object_count);
		return ret.build();
	}
	
	class Bitmap
	{
		public int w;
		public int h;
		public int c=3;
		public byte[] data;
	}
}