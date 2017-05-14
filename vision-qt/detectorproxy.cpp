#include "detectorproxy.h"
#include <net_thread.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
DetectorProxy::DetectorProxy()
{
    thread = new NetThread();
}

void DetectorProxy::run()
{
    connect(thread,SIGNAL(recWeightMap(ITCMsg*)),this,SLOT(getWeightMap(ITCMsg*))) ;
    connect(thread,SIGNAL(recFeature(ITCMsg*)),this,SLOT(getFeatureMap(ITCMsg*))) ;
    connect(thread,SIGNAL(recLayerInfo(ITCMsg*)),this,SLOT(getLayerInfo(ITCMsg*))) ;
    connect(thread,SIGNAL(recInput(ITCMsg*)),this,SLOT(getInput(ITCMsg*))) ;
    connect(thread,SIGNAL(recOutput(ITCMsg*)),this,SLOT(getOutput(ITCMsg*))) ;
    connect(thread,SIGNAL(recDeconv(ITCMsg*)),this,SLOT(getDeconv(ITCMsg*))) ;
    thread->start();
    thread->getSendThread()->start();
    exec();
}

SendThread* DetectorProxy::getSendThread()
{
    return thread->getSendThread();
}

void DetectorProxy::getFeatureMap(ITCMsg* itcmsg)
{
    VFeatureMap *map = (VFeatureMap*)itcmsg->msg;
    unsigned char* weights = (unsigned char*)const_cast<char*>(map->mutable_data()->c_str());
    int w = map->width();
    int h = map->height();
    int c = map->channel();
    int n = map->num();
    int size = w*h;

    int res = 2;
    if(w>=3)res = 4;
    if(w>=5)res = 8;
    if(w>10)res = 20;
    if(w>20)res = 40;
    if(w>50)res = 80;
    if(w>100)res = 160;
    if(w>150)res = 200;
    unsigned char *map_data = new unsigned char[c*n*res*res];
    if(c==3)
    {
        int image_index=0;
        //以下是设置第一行的所有列的图片
         for(int i=0; i<n; i++)
         {
             cv::Mat image(h,w,CV_8UC3);
             for(int a=0;a<h;++a)
             {
                 for(int j=0;j<w;++j)
                 {
                     for(int k=0;k<3;++k)
                     {
                         image.data[image_index++] = weights[k*w*h+h*a+j+res*res*c*i];
                     }
                 }
             }
             //memcpy(image.data,weights+i*w*h,w*h);
             cv::Mat dst;
             cv::resize(image, dst, cv::Size(res, res ),(0, 0), (0, 0), cv::INTER_LINEAR);
             memcpy(map_data+res*res*i*c,dst.data,3*res*res*sizeof(unsigned char));
         }
    }else{
        //以下是设置第一行的所有列的图片
         for(int i=0; i<c*n; i++)
         {
             cv::Mat image(h,w,CV_8UC1);
             memcpy(image.data,weights+i*w*h,w*h);
             cv::Mat dst;
             cv::resize(image, dst, cv::Size(res, res ),(0, 0), (0, 0), cv::INTER_LINEAR);
             memcpy(map_data+res*res*i,dst.data,res*res*sizeof(unsigned char));
         }
    }


    emit this->recFeature(res,res,c,n,map_data);
    delete itcmsg;

}

void DetectorProxy::getDeconv(ITCMsg* itcmsg)
{
    VImage *im = (VImage*)itcmsg->msg;
    int w = im->width();
    int h = im->height();
    int c = im->channel();
    unsigned char* weights = (unsigned char*)const_cast<char*>(im->data().data());
    int image_index = 0;
    cv::Mat rgb(h,w,CV_8UC3);
    for(int i=0;i<h;++i)
    {
        for(int j=0;j<w;++j)
        {
            for(int k=0;k<3;++k)
            {
                rgb.data[image_index++] = weights[k*w*h+h*i+j];
            }
        }
    }
    int num = 0;//weishu
    while(w>10)
    {
        w/=10;
        num++;
    }
    for(int i=0;i<num;++i)w*=10;
    while(h>10)
    {
        h/=10;
        num++;
    }
    for(int i=0;i<num;++i)h*=10;
    cv::Mat dst;
    cv::resize(rgb, dst, cv::Size(w, h ),(0, 0), (0, 0), cv::INTER_LINEAR);
    unsigned char* image = new unsigned char[w*h*c];
    memcpy(image,dst.data,w*h*c);
    emit this->recDeconv(w,h,c,image);
    delete itcmsg;
}

void DetectorProxy::getInput(ITCMsg* itcmsg)
{
    VImage *im = (VImage*)itcmsg->msg;
    int w = im->width();
    int h = im->height();
    int c = im->channel();
    unsigned char* weights = (unsigned char*)const_cast<char*>(im->data().c_str());
    cv::Mat rgb(h,w,CV_8UC3);
    int image_index = 0;
    for(int i=0;i<h;++i)
    {
        for(int j=0;j<w;++j)
        {
            for(int k=2;k>=0;k--)
            {
                rgb.data[image_index++] = weights[k*w*h+h*i+j];
            }
        }
    }
    int num = 0;//weishu
    while(w>10)
    {
        w/=10;
        num++;
    }
    for(int i=0;i<num;++i)w*=10;
    while(h>10)
    {
        h/=10;
        num++;
    }
    for(int i=0;i<num;++i)h*=10;
    cv::Mat dst;
    cv::resize(rgb, dst, cv::Size(w, h ),(0, 0), (0, 0), cv::INTER_LINEAR);
    unsigned char* image = new unsigned char[w*h*c];
    memcpy(image,dst.data,w*h*c);
    emit this->recInput(w,h,c,image);
    delete itcmsg;
}

void DetectorProxy::getLayerInfo(ITCMsg*itcmsg)
{
    emit this->recLayerInfo(itcmsg);
   // delete itcmsg;
}

void DetectorProxy::getOutput(ITCMsg* itcmsg)
{

}

void DetectorProxy::getWeightMap(ITCMsg* itcmsg)
{
    VFeatureMap *map = (VFeatureMap*)itcmsg->msg;
    unsigned char* weights = (unsigned char*)const_cast<char*>(map->mutable_data()->c_str());
    int w = map->width();
    int h = map->height();
    int c = map->channel();
    int n = map->num();
    int size,num;

    int w_h_scale;
    cv::Mat data;
    cv::Mat scal_data;
    if(c==3){
        w_h_scale  = 4;
        size = c*w*h;
        num = n;
        data.create(h,w,CV_8UC3);
        scal_data.create(h,w,CV_8UC3);
    }else{
        w_h_scale = 4;
        size = w*h;
        num = n*c;
        data.create(h,w,CV_8UC1);
        scal_data.create(h*w_h_scale,w*w_h_scale,CV_8UC1);
    }

    unsigned char* tran_weight = new unsigned char[num*size*w_h_scale*w_h_scale];
   //以下是设置第一行的所有列的图片
    for(int n=0; n<num; ++n)
    {
        unsigned char* image ;
        image = data.data;
        int image_index = 0;
        for(int i=0;i<h;++i)
        {
            for(int j=0;j<w;++j)
            {
                if(c==3)
                {
                    for(int k=0;k<3;++k)
                    {
                        image[image_index++] = (weights+n*size)[k*w*h+h*i+j];
                    }
                }else{
                    image[image_index++] = (weights+n*size)[h*i+j];
                }
            }
        }
        cv::resize(data,scal_data,cv::Size(w*w_h_scale,h*w_h_scale),cv::INTER_NEAREST);
        memcpy(tran_weight+size*n*w_h_scale*w_h_scale,scal_data.data,size*w_h_scale*w_h_scale);
    }
    //delete []tran_weight;
    emit this->recWeightMap(w*w_h_scale,h*w_h_scale,c,n,tran_weight);
    delete itcmsg;

}
