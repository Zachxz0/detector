#include <media/mmedia_trans.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace std;
#include <iostream>
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

namespace zoson
{

void DisplayYUV(unsigned char**data,int w,int h)  
{  
        cv::Mat yuvImg;  
        yuvImg.create(h*3/2, w, CV_8UC1);   
        memcpy(yuvImg.data, data[0], w*h*3*sizeof(unsigned char)/2);  
        cv::Mat rgbImg;  
        cv::cvtColor(yuvImg, rgbImg, CV_YUV2BGR_I420); 
        cv::imshow("img",rgbImg);
        cv::waitKey(1);
    //    shared_ptr<DetectOutput> out =  dec.doDetect(rgbImg);
    // string out_str;
    // TextFormat::PrintToString(*out,&out_str);
   // cout<<out_str<<endl;
}  


MMediaUtils::MMediaUtils(const MMediaParameter& parma)
{

}

MMediaUtils::MMediaUtils(const string& param)
{

}

MMediaUtils::~MMediaUtils()
{

}

void MMediaUtils::init()
{

}

void MMediaUtils::deInit()
{

}

int MMediaUtils::trans()
{
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
    double  time_duration; 

	char info[1000]={0};  
  
    //FFmpeg av_log() callback  

    av_register_all();  
    avformat_network_init();  
    pFormatCtx = avformat_alloc_context();  
  
    if(avformat_open_input(&pFormatCtx,"rtmp://192.168.31.208/test",NULL,NULL)!=0){  
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
                DisplayYUV(pFrameYUV->data,pCodecCtx->width,pCodecCtx->height);
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
DisplayYUV(pFrameYUV->data,pCodecCtx->width,pCodecCtx->height);
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
}

void MMediaUtils::setCallback(CallbackForData *cb)
{
	this->mCallback = cb;
}

}