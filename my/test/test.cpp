#include <iostream>
#include <string>
#include <fstream>
#include <proto/zoson.pb.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <detector/detector.hpp>
#include <commu/commu.hpp>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <net/server.h>
#include <net/asio_server.hpp>
#include <net/asio_client.hpp>
//#include <commu/commu.hpp>

#include <boost/thread/thread.hpp>
#include <glog/logging.h>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <app/dcontext.hpp>
//#include <boost/thread/detail/lock.hpp>

//ffmpeg 
extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/log.h"
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
#include <libavutil/mathematics.h>
}
//

using namespace cv;
using namespace google::protobuf;
using namespace google::protobuf::io;
using namespace std;
using namespace zoson;


//caffe
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "boost/algorithm/string.hpp"
#include "caffe/caffe.hpp"
#include "caffe/util/signal_handler.h"
//#include "caffe/layers/zoson_data_layer.hpp"

using caffe::Blob;
using caffe::Caffe;
using caffe::Net;
using caffe::Layer;
using caffe::Solver;
using caffe::shared_ptr;
using caffe::string;
using caffe::Timer;
using caffe::vector;
using std::ostringstream;
using namespace caffe;
//caffe

bool test_zoson_module_json();
bool test_zoson_module_proto();
bool test_zoson_module_detector();
bool test_ffmpeg_rtmp();
bool test_detecor_train();
void WriteYuv();
bool test_net_proto();
bool test_glog();
bool test_commu();
bool testAsio();
bool test_dcontext();

void DisplayYUV(Detector&, unsigned char**,int,int);
int main()
{

	if(test_zoson_module_proto()){
		cout<<"test_zoson_module_proto"<<" true"<<endl;
	}else{
		cout<<"test_zoson_module_proto"<<" false"<<endl;
	}
	// if(test_zoson_module_detector())
	// {
	// 	cout<<"test_zoson_module_detector"<<" true"<<endl;
	// }else{
	// 	cout<<"test_zoson_module_detector"<<" false"<<endl;
	// }
	//test_ffmpeg_rtmp();
	//WriteYuv();
	//DisplayYUV();
    //test_detecor_train();
    //test_net_proto();
    //test_glog();
    //test_commu();
    //testAsio();
    test_dcontext();
}


bool test_zoson_module_proto()
{
	bool re = false;
	TestProto testProto ;
	testProto.set_clazz_num(10);
	testProto.add_dim(11);
	string se;
	TextFormat::PrintToString(testProto,&se);
	TestProto copy;
	TextFormat::ParseFromString(se,&copy);
	fstream strin("config/test.prototxt",ios::out);
	strin.write(se.c_str(),se.size());
	strin.close();

	fstream output("config/test.protobuf",ios::out);
	testProto.SerializeToOstream(&output);
	output.close();
	
	fstream input("config/test.protobuf",ios::in);
	TestProto d2;
	d2.ParseFromIstream(&input);
	if(d2.clazz_num()==10&&d2.dim(0)==11){
		re = true;
	}
	input.close();

	ifstream in("config/test.prototxt");
	string instr;
	char buf[64];
	while(!in.eof())
	{
		in.read(buf,64);
		instr.append(buf);
	}
	in.close();
	TestProto de;
	TextFormat::ParseFromString(instr,&de);
	if(de.clazz_num()==10&&de.dim(0)==11){
		re = true;
	}
	return re;
}

char* imageName[3] = {"./data/test/ly.jpg","./data/test/cat.jpg","./data/test/ly.png"};
Mat getCVMatImage(int i)
{
	Mat image = imread( imageName[0], CV_LOAD_IMAGE_COLOR);
	return image;
}

bool test_zoson_module_detector(){
	Detector dec("./config/detector.prototxt");
	dec.initForTest();
	for(int i=0;i<10000;i++)
	{
		shared_ptr<DetectOutput> out = dec.doDetect(getCVMatImage(i));
	string out_str;
	TextFormat::PrintToString(*out,&out_str);
	cout<<out_str<<endl;
	}
	dec.deInitForTest();
	return true;
}


void detect_rgb(Detector &dec,unsigned char** rgb_data,int len,int w,int h)
{
	if(rgb_data==NULL)cout<<"rgb == null"<<endl;
	//unsigned char* data = new unsigned char[len*sizeof(unsigned char)*3];
	//memcpy(data,rgb_data[0]+len*2*sizeof(unsigned char),len*sizeof(unsigned char));
	///memcpy(data+len*sizeof(unsigned char),rgb_data[0]+len*sizeof(unsigned char),len*sizeof(unsigned char));
	////memcpy(data+len*sizeof(unsigned char)*2,rgb_data[0],len*sizeof(unsigned char));
	cv::Mat rgbImg;  
	rgbImg.create(h, w, CV_8UC3);   
	memcpy(rgbImg.data,rgb_data, len*3*sizeof(unsigned char));  
	cv::imshow("img",rgbImg);
	//dec.doDetect(rgbImg);
	cv::waitKey(1); 
	//delete[] data;
}

bool test_ffmpeg_rtmp()
{
	Detector dec("./config/detector.prototxt");
	//dec.init();

	AVFormatContext *pFormatCtx;  
    int             i, videoindex;  
    AVCodecContext  *pCodecCtx;  
    AVCodec         *pCodec;  
    AVFrame *pFrame,*pFrameYUV;  
    uint8_t *out_buffer;  
    AVPacket *packet;  
    struct SwsContext *img_convert_ctx;  
    int y_size;  
    int ret, got_picture;  
    FILE *fp_yuv;  
    int frame_cnt;  
    clock_t time_start, time_finish;  
    double  time_duration = 0.0;  
  

    char info[1000]={0};  
  
    //FFmpeg av_log() callback  
      
    av_register_all();  
    avformat_network_init();  
    pFormatCtx = avformat_alloc_context();  
  
    if(avformat_open_input(&pFormatCtx,"rtmp://localhost/live/test",NULL,NULL)!=0){  
        cout<<"Couldn't open input stream.\n";  
        return -1;  
    }  
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){  
        cout<<"Couldn't find stream information.\n";  
        return -1;  
    }  
    videoindex=-1;  
    for(i=0; i<pFormatCtx->nb_streams; i++)   
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){  
            videoindex=i;  
            break;  
        }  
    if(videoindex==-1){  
        cout<<"Couldn't find a video stream.\n";  
        return -1;  
    }  
    pCodecCtx=pFormatCtx->streams[videoindex]->codec;  
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);  
    if(pCodecCtx->codec_id==AV_CODEC_ID_H264){
    	cout<<"Codec is H264"<<endl;
    }
    if(pCodec==NULL){  
        cout<<"Couldn't find Codec.\n";  
        return -1;  
    }  
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){  
        cout<<"Couldn't open codec.\n";  
        return -1;  
    }  
      
    pFrame=av_frame_alloc();  
    pFrameYUV=av_frame_alloc();  
    out_buffer=(unsigned char *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P,  pCodecCtx->width, pCodecCtx->height));  
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize,out_buffer,  AV_PIX_FMT_YUV420P,pCodecCtx->width, pCodecCtx->height,1);  
   // out_buffer=(unsigned char *)av_malloc(avpicture_get_size(AV_PIX_FMT_RGB24,  pCodecCtx->width, pCodecCtx->height)); 
             //avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);  
      
    packet=(AVPacket *)av_malloc(sizeof(AVPacket));  
     
  
    
  sprintf(info, "%s[Format    ]%s\n",info, pFormatCtx->iformat->name);  
  sprintf(info, "%s[Codec     ]%s\n",info, pCodecCtx->codec->name);  
  sprintf(info, "%s[Resolution]%dx%d\n",info, pCodecCtx->width,pCodecCtx->height);  
  
      
    frame_cnt=0;  
    time_start = clock();  
         //img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);   
img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,   pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    while(av_read_frame(pFormatCtx, packet)>=0){  
        if(packet->stream_index==videoindex){  
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);  
            if(ret < 0){  
                cout<<"Decode Error.\n";  
                return -1;  
            }  
            if(got_picture){  
                sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,   pFrameYUV->data, pFrameYUV->linesize);  
                  //sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,   pFrameYUV->data, pFrameYUV->linesize);
                y_size=pCodecCtx->width*pCodecCtx->height; 
                DisplayYUV(dec,pFrameYUV->data,pCodecCtx->width,pCodecCtx->height);
                //detect_rgb(dec,pFrameYUV->data,y_size,pCodecCtx->width,pCodecCtx->height);
                // fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y   
                // fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U  
                // fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V  
                //Output info  
                char pictype_str[10]={0};  
                switch(pFrame->pict_type){  
                    case AV_PICTURE_TYPE_I:sprintf(pictype_str,"I");break;  
                  case AV_PICTURE_TYPE_P:sprintf(pictype_str,"P");break;  
                    case AV_PICTURE_TYPE_B:sprintf(pictype_str,"B");break;  
                    default:sprintf(pictype_str,"Other");break;  
                }  
               // cout<<"Frame Index: %5d. Type:%s",frame_cnt,pictype_str;  
                frame_cnt++;  
            }  
        }  
        av_free_packet(packet);  
    }  
    //flush decoder  
    //FIX: Flush Frames remained in Codec  
    while (1) {  
        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);  
        if (ret < 0)  
            break;  
        if (!got_picture)  
            break;  
        //sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,   pFrameYUV->data, pFrameYUV->linesize);
        sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,   pFrameYUV->data, pFrameYUV->linesize);  
        int y_size=pCodecCtx->width*pCodecCtx->height;    
       // y_size=pCodecCtx->width*pCodecCtx->height; 
DisplayYUV(dec,pFrameYUV->data,pCodecCtx->width,pCodecCtx->height);
        //rgb transf 
                    //sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
       //detect_rgb(dec,pFrameYUV->data,y_size,pCodecCtx->width,pCodecCtx->height);
        // fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y   
        // fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U  
        // fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V  
        //Output info  
        char pictype_str[10]={0};  
        switch(pFrame->pict_type){  
            case AV_PICTURE_TYPE_I:sprintf(pictype_str,"I");break;  
          case AV_PICTURE_TYPE_P:sprintf(pictype_str,"P");break;  
            case AV_PICTURE_TYPE_B:sprintf(pictype_str,"B");break;  
            default:sprintf(pictype_str,"Other");break;  
        }  
        //cout<<"Frame Index: %5d. Type:%s",frame_cnt,pictype_str;  
        frame_cnt++;  
    }  
    time_finish = clock();   
    time_duration=(double)(time_finish - time_start);  
      
    sprintf(info, "%s[Time      ]%fms\n",info,time_duration);  
    sprintf(info, "%s[Count     ]%d\n",info,frame_cnt);  
  
    sws_freeContext(img_convert_ctx);  
  
  //fclose(fp_yuv);  
  
    av_frame_free(&pFrameYUV);  
    av_frame_free(&pFrame);  
    avcodec_close(pCodecCtx);  
    avformat_close_input(&pFormatCtx);  
  
    return 0;  
}

int w,h;
void WriteYuv()  
{  
    cv::VideoCapture vc;  
    bool flag = vc.open("./data/test/an.avi");  
    if (!flag)  
    {  
        printf("avi file open error n");  
        //system("pause");  
        //exit(-1);  
    }  
  
    int frmCount = vc.get(CV_CAP_PROP_FRAME_COUNT);  
    frmCount -= 5;  
    printf("frmCount: %d n", frmCount);  
  
    w = vc.get(CV_CAP_PROP_FRAME_WIDTH);  
    h = vc.get(CV_CAP_PROP_FRAME_HEIGHT);  
    int bufLen = w*h*3/2;  
    unsigned char* pYuvBuf = new unsigned char[bufLen];  
    FILE* pFileOut = fopen("./data/test/result.yuv", "w+");  
    if (!pFileOut)  
    {  
        printf("pFileOut open error n");  
        //system("pause");  
        //exit(-1);  
    }  
    printf("pFileOut open ok n");  
      
    for (int i=0; i<500; i++)  
    {  
        printf("%d/%d n", i+1, frmCount);  
  
        cv::Mat srcImg;  
        vc>>srcImg;  
  
        cv::imshow("img", srcImg);  
        cv::waitKey(1);  
  
        cv::Mat yuvImg;  
        cv::cvtColor(srcImg, yuvImg, CV_BGR2YUV_I420);  
        memcpy(pYuvBuf, yuvImg.data, bufLen*sizeof(unsigned char));  
  
        fwrite(pYuvBuf, bufLen*sizeof(unsigned char), 1, pFileOut);  
    }  
  
    fclose(pFileOut);  
    delete[] pYuvBuf;  
}  

void DisplayYUV(Detector&dec, unsigned char**data,int w,int h)  
{  
        cv::Mat yuvImg;  
        yuvImg.create(h*3/2, w, CV_8UC1);   
        memcpy(yuvImg.data, data[0], w*h*3*sizeof(unsigned char)/2);  
        cv::Mat rgbImg;  
        cv::cvtColor(yuvImg, rgbImg, CV_YUV2BGR_I420); 
        cv::imshow("img",rgbImg);
        cv:waitKey(1);
    //    shared_ptr<DetectOutput> out =  dec.doDetect(rgbImg);
    // string out_str;
    // TextFormat::PrintToString(*out,&out_str);
   // cout<<out_str<<endl;
}  

bool test_detecor_train()
{
    Detector dec("./config/detector_train.prototxt");
    dec.initForTrain();
    return true;
}

// Mat trans(Blob<float> *data)
// {

//     Dtype* transformed_data = data->mutable_cpu_data();
//   int top_index;
//   for (int h = 0; h < height; ++h) {
//     const uchar* ptr = cv_cropped_image.ptr<uchar>(h);
//     int img_index = 0;
//     int h_idx = h;
//     for (int w = 0; w < width; ++w) {
//       int w_idx = w;
//       if (*do_mirror) {
//         w_idx = (width - 1 - w);
//       }
//       int h_idx_real = h_idx;
//       int w_idx_real = w_idx;
//       for (int c = 0; c < img_channels; ++c) {
//         top_index = (c * height + h_idx_real) * width + w_idx_real;
//         Dtype pixel = static_cast<Dtype>(ptr[img_index++]);
//         if (has_mean_file) {
//           int mean_index = (c * img_height + h_off + h_idx_real) * img_width
//               + w_off + w_idx_real;
//           transformed_data[top_index] =
//               (pixel - mean[mean_index]) * scale;
//         } else {
//           if (has_mean_values) {
//             transformed_data[top_index] =
//                 (pixel - mean_values_[c]) * scale;
//           } else {
//             transformed_data[top_index] = pixel * scale;
//           }
//         }
//       }
//     }
// }
Server* server;
boost::mutex io_mutex;
boost::mutex rec_mutex;
int rec_state = 0;
void senddata(boost::shared_ptr<VResponse> rep_ptr,int nocontin = 0)
{
    boost::mutex::scoped_lock lock(io_mutex);
    string out;
    rep_ptr->SerializeToString(&out);
    string count;
    Bytetranfer bytes;
    bytes.set_count(out.size());
    bytes.SerializeToString(&count);
    // if(!nocontin)
    // {
    //     boost::mutex::scoped_lock rec_lock(rec_mutex);
    //     if(rec_state==0)
    //     {
    //         cout<<"send contin";
    //         server->sendMessage(count.c_str(),count.size());
    //         usleep(1);
    //         server->sendMessage(out.c_str(),out.size());
    //         rec_state=-1;
    //     }
    // }else{
        server->sendMessage(count.c_str(),count.size());
        usleep(1);
        server->sendMessage(out.c_str(),out.size());
    //}
    
}


Detector *detector;
class Cb:public ServerCallback
{
public:
    void getClientMessage(int fd,const char *buf,int size){
        VResponse req ;
        string in(buf,size);
        req.ParseFromString(in);
        switch(req.type()){
            case VResponse_Type_FEATUREMAP:
            {
                VReqFeature req_f;
                req_f.ParseFromString(req.data());
                boost::shared_ptr<VResponse> rep_ptr(new VResponse);
                detector->getFeatureMap(req_f,rep_ptr.get());
                string out;
                rep_ptr->SerializeToString(&out);
                server->sendMessage(out.c_str(),out.size());
            }
            break;
            case VResponse_Type_LAYERINFOS:
            {
                boost::shared_ptr<VResponse> rep_ptr(new VResponse);
                detector->getDeconvAbleLayers(rep_ptr.get());
                senddata(rep_ptr,1);
                cout<<"VResponse_Type_LAYERINFOS "<<endl;
                // string out;
                // rep_ptr->SerializeToString(&out);
                // string count;
                // Bytetranfer bytes;
                // bytes.set_count(out.size());
                // bytes.SerializeToString(&count);
                // cout<<"send:"<<bytes.count()<<endl;
                // server->sendMessage(count.c_str(),count.size());
                // usleep(1);
                // server->sendMessage(out.c_str(),out.size());
            }
            break;
            case VResponse_Type_WEIGHT:
            {
                boost::shared_ptr<VResponse> rep_ptr(new VResponse());
                VReqWeight req_w ;
                req_w.ParseFromString(req.data());
                detector->getWeightForTest(req_w,rep_ptr.get());
                string out;
                rep_ptr->SerializeToString(&out);
                server->sendMessage(out.c_str(),out.size());
            }
            break;
            case VResponse_Type_INPUT:
            {
                boost::shared_ptr<VResponse> rep_ptr(new VResponse);
                detector->getOriginalImage(0,rep_ptr.get());
                string out;
                rep_ptr->SerializeToString(&out);
                server->sendMessage(out.c_str(),out.size());
            }
            break;
            case VResponse_Type_STATE:
            {
                VState state ;
                state.ParseFromString(req.data());
                string msg;
                TextFormat::PrintToString(state,&msg);
                detector->setState(state);;
            }
            break;
            case VResponse_Type_REC:
            {
                boost::mutex::scoped_lock lock(rec_mutex);
                VRecState vrec_state;
                vrec_state.ParseFromString(req.data());
                if(vrec_state.state()==0){
                    rec_state = 0;
                }else{
                    rec_state = -1;
                }
                cout<<"VResponse_Type_REC "<<rec_state<<endl;
            }
            break;
        }
    }

    void hasClientConn(int fd)
    {

    }

    void hasClientClose(int fd)
    {
        
    }
};

class DecCb: public Detector::Callback
{
public:
    void haveMesssage(boost::shared_ptr<VResponse> msg)
    {
        senddata(msg);
        // string out;
        // msg->SerializeToString(&out);

        // string count;
        // Bytetranfer bytes;
        // bytes.set_count(out.size());
        // bytes.SerializeToString(&count);
        // cout<<"send:"<<bytes.count()<<endl;
        // server->sendMessage(count.c_str(),count.size());
        // usleep(1);
        // cout<<"out:"<<out.size()<<endl;
        // server->sendMessage(out.c_str(),out.size());
        //if(out.size()>6336960)usleep(100);
    }
};

void serverConn()
{
    server = new Server();
    server->setCallback(new Cb());
    server->startServer();
}

bool  test_net_proto()
{
    boost::thread th(&serverConn);

    // detector = new Detector("./config/train.prototxt");
    //     detector->addListeners(new DecCb());
    // detector->initForTrain();

    detector = new Detector("./config/googlenet.prototxt");
    detector->initForTest();
    detector->addListeners(new DecCb());

    for(int i=0;i<1000000;i++)
    {
        Mat get = getCVMatImage(i);
        //detector->doDetect(get);
        while(1){
            //detector->on_gradients_ready();
            sleep(2);
        }
    }
    th.join();
    
}

bool test_glog()
{
    //google::InitGoogleLogging();
    LOG(INFO)<<"dsf"<<endl;
}

bool test_commu()
{
    // Communicator commu("./config/commu.prototxt");
    // commu.connect();
    return false;
}

void asio_server()
{
    boost::asio::io_service io_service;
    tcp::endpoint endpoint(address::from_string("192.168.31.206"), 10028);
    AsioServer ser(io_service, endpoint);
    ser.run();
}

void asio_client()
{
    boost::asio::io_service io_service;
    tcp::endpoint endpoint(address::from_string("192.168.31.206"), 10028);

    client_session_ptr new_session(new client_session(io_service, endpoint));
    new_session->start();
    io_service.run();
}

bool testAsio()
{
    boost::thread th(&asio_server);
    //sleep(1);
    //boost::thread th2(&asio_client);
    th.join();
    //th2.join();
}

bool test_dcontext()
{
    //boost::thread th(&serverConn);
    DContext dcontext("./config/dcontext.prototxt");
    shared_ptr<Detector> detector_ptr = dcontext.getDetector("detector");
    detector = detector_ptr.get();
    shared_ptr<Communicator> commu_ptr =  dcontext.getCommu();
    commu_ptr->connect();
    if(!detector->ifTrain())
    {
        detector->initForTest();
        //detector->addListeners(new DecCb());

        for(int i=0;i<1000000;i++)
        {
            Mat get = getCVMatImage(i);
            shared_ptr<DetectOutput> ptr =  detector->doDetect(get);
            int size = ptr->results_size();
            cout<<"size:"<<size<<endl;
             cout<<"+++++++++++++++++"<<endl;
            for(int i=0;i <size;++i)
            {
                cout<<"-----------------"<<endl;
                const Result &res = ptr->results(i);
                cout<<"x max:"<<res.xmax()<<endl;
                cout<<"x min:"<<res.xmin()<<endl;
                cout<<"y max:"<<res.ymax()<<endl;
                cout<<"y min:"<<res.ymin()<<endl;
                cout<<"score :"<<res.score()<<endl;
                cout<<"class:"<<res.clazz()<<endl;
                cout<<"-----------------"<<endl;
            }
            cout<<"+++++++++++++++++"<<endl;
            //while(1){
                detector->on_gradients_ready();
                sleep(5);
           // }
        }
    }else{
        //detector->addListeners(new DecCb());
        detector->initForTrain();
    }
    commu_ptr->join();
    //th.join();
}