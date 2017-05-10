#include "net_thread.h"
#include "net.hpp"
#include <iostream>
#include <string>
#include <google/protobuf/text_format.h>

using namespace google::protobuf;
using namespace std;
NetThread::NetThread()
{
    this->client = new zoson::NetClient();
    sendThread = new SendThread(this->client);
}

NetThread::NetThread(zoson::NetClient*client){
    this->client = client;
}

void NetThread::run()
{
    zoson::ConnInfo info;
    info.server_addr = "192.168.31.206";
    info.server_post = 10200;
    info.cookies = "qt";
    this->client->connectServer(info);
    bool is =true;
    zoson::Data data;
    Bytetranfer bytes ;
    bool bytesget = false;

    //for validation
    string ser;
    Bytetranfer forcount;
    forcount.set_count(-1);
    forcount.SerializeToString(&ser);

    while(is){
//        VResponse rep_state;
//        rep_state.set_type(VResponse_Type_REC);
//        VRecState vrec_state;
//        vrec_state.set_state(0);
//        string state_str;
//        vrec_state.SerializeToString(&state_str);
//        rep_state.set_data(state_str);
//        string rep_str;
//        rep_state.SerializeToString(&rep_str);
//        zoson::Data state_data;
//        state_data.load_data((unsigned char*)const_cast<char*>(rep_str.data()),rep_str.size());
//        client->sendData(&state_data);

        is = this->client->recData(&data);
        string str_data = (char*)data.buf;
        bytes.ParseFromString(str_data);
        is &= this->client->recData(&data,bytes.count());

        string msg = (char*)data.buf;
        VResponse rep  ;
        is &=rep.ParseFromString(msg);
        if(!is){
            cout<<"flush"<<endl;
            //client->flush();
            is = true;
            continue;
        }
        handleResponse(&rep);
    }
    exec();
}

void NetThread::handleResponse(const VResponse* rep)
{
    switch(rep->type())
    {
        case VResponse_Type_FEATUREMAP:
        {
            VFeatureMap *map = new VFeatureMap;
            if(!map->ParseFromString(rep->data()))
            {
                cerr << "VResponse_Type_FEATUREMAP Failed to parse VFeatureMap." << endl;
                return;
            }
            ITCMsg *itcmsg = new ITCMsg;
            itcmsg->msg = map;
            emit recFeature(itcmsg);
            //delete itcmsg;

        }
        break;
        case VResponse_Type_WEIGHT:
        {
            VFeatureMap *map = new VFeatureMap;
            if(!map->ParseFromString(rep->data()))
            {
                cerr << "VResponse_Type_WEIGHT Failed to parse VFeatureMap." << endl;
                return;
            }
            ITCMsg* itcmsg = new ITCMsg;
            itcmsg->msg = map;
            emit recWeightMap(itcmsg);
            //delete itcmsg;
        }
        break;
        case VResponse_Type_LAYERINFOS:
        {
            VLayerInfos *infos = new VLayerInfos;
            if(!TextFormat::ParseFromString(rep->data(),infos))
            {
                cerr << "VResponse_Type_LAYERINFOS Failed to parse VLayerInfos." << endl;
                return;
            }
            ITCMsg* itcmsg = new ITCMsg;
            itcmsg->msg = infos;
            emit recLayerInfo(itcmsg);
           // delete itcmsg;

        }
        break;
        case VResponse_Type_DECONV:
        {
            VImage* im = new VImage;
            if(!im->ParseFromString(rep->data()))
            {
                cerr << "VResponse_Type_OUTPUT Failed to parse VImage." << endl;
                return;
            }
            ITCMsg* itcmsg = new ITCMsg;
            itcmsg->msg = im;
            emit recDeconv(itcmsg);
           // delete itcmsg;

        }
        break;
        case VResponse_Type_INPUT:
        {
            VImage* im = new VImage;
            if(!im->ParseFromString(rep->data()))
            {
                cerr << "VResponse_Type_INPUT Failed to parse VImage." << endl;
                return;
            }
            ITCMsg* itcmsg = new ITCMsg;
            itcmsg->msg = im;
            emit recInput(itcmsg);
            //delete itcmsg;

        }
        break;
                case VResponse_Type_ALL:
        {
            VAll all ;
            if(!all.ParseFromString(rep->data()))
            {
                cerr << "VResponse_Type_ALL Failed to parse VAll." << endl;
                return;
            }
            int size = all.response_size();
            for(int i=0;i<size;++i)
            {
                handleResponse(&all.response(i));
            }
        }
        break;
    default:
    {}
    }
}

void NetThread::getLayerInfos()
{
    zoson::Data data;
    VResponse rep;
    rep.set_type(VResponse_Type_LAYERINFOS);
    string out;
    rep.SerializeToString(&out);
    data.load_data((unsigned char*)(const_cast<char*>(out.c_str())),out.size());
    client->sendData(&data);
}

SendThread::SendThread(zoson::NetClient *client)
{
    this->client = client;
}

void SendThread::requestFeature(int i_layer,int i_image,bool isDiff)
{
    cout<<"requestFeature"<<endl;
    zoson::Data senddata;
    VResponse resp;
    resp.set_type(VResponse_Type_STATE);
    string state_str;
    VReqFeature *req = vstate.mutable_map();
    if(i_layer!=-1)
    {
        req->set_i_layer(i_layer);
    }else if(req->i_layer()<0){
        i_layer = 0;
        req->set_i_layer(0);
    }else{
        i_layer = req->i_layer();
    }
    if(i_image!=-1)
    {
        req->set_i_image(i_image);
    }else if(req->i_image()<0){
        i_layer = 0;
        req->set_i_image(0);
    }else{
        i_image = req->i_image();
    }
    VReqWeight *req_weight = vstate.mutable_weight();
    req_weight->set_index(i_layer);

    req->set_diff(isDiff);
    vstate.SerializeToString(&state_str);
    resp.set_data(state_str.c_str(),state_str.size());
    string out;
    resp.SerializeToString(&out);
    senddata.load_data((unsigned char*)const_cast<char*>(out.c_str()),out.size());
    client->sendData(&senddata);

//    zoson::Data data;
//    VResponse rep ;
//    VReqFeature req_feature;
//    if(i_layer==0){
//        rep.set_type(VResponse_Type_WEIGHT);
//        req_feature.set_i_image(i_image);
//        req_feature.set_i_layer(0);
//    }else if(i_layer>=1){
//        rep.set_type(VResponse_Type_FEATUREMAP);
//        req_feature.set_i_image(i_image);
//        req_feature.set_i_layer(i_layer-1);
//    }
//    string req_feature_out;
//    string response_out;
//    req_feature.SerializeToString(&req_feature_out);
//    rep.set_data(req_feature_out);
//    rep.SerializeToString(&response_out);
//    data.size = response_out.size();
//    data.buf = new unsigned char[data.size];
//    memcpy( data.buf,(response_out.c_str()),data.size);
//    client->sendData(&data);
}

void SendThread::requestLayerInfo(int )
{
    zoson::Data data;
    VResponse rep;
    rep.set_type(VResponse_Type_LAYERINFOS);
    string out;
    rep.SerializeToString(&out);
    data.load_data((unsigned char*)const_cast<char*>(out.c_str()),out.size());
    client->sendData(&data);
}

void SendThread::requestWeight(int i)
{
    zoson::Data senddata;
    VResponse resp;
    resp.set_type(VResponse_Type_STATE);
    string state_str;
    VReqWeight *req = vstate.mutable_weight();
    req->set_index(i);
    vstate.SerializeToString(&state_str);
    resp.set_data(state_str.c_str(),state_str.size());
    string out;
    resp.SerializeToString(&out);
    senddata.load_data((unsigned char*)const_cast<char*>(out.c_str()),out.size());
    client->sendData(&senddata);

//    zoson::Data senddata;
//    VResponse resp;
//    resp.set_type(VResponse_Type_WEIGHT);
//    string out;
//    resp.SerializeToString(&out);
//    senddata.size = out.size();
//    senddata.buf = new unsigned char[senddata.size];
//    memcpy(senddata.buf,(out.c_str()),senddata.size);
//    client->sendData(&senddata);
}

void SendThread::requestInput(int i)
{
    zoson::Data senddata;
    VResponse resp;
    resp.set_type(VResponse_Type_STATE);
    string state_str;
    vstate.set_input(i);
    vstate.SerializeToString(&state_str);
    resp.set_data(state_str.c_str(),state_str.size());
    string out;
    resp.SerializeToString(&out);
    senddata.load_data((unsigned char*)const_cast<char*>(out.c_str()),out.size());
    client->sendData(&senddata);
    cout<<"size "<<senddata.size<<endl;
//    zoson::Data senddata;
//    VResponse resp;
//    resp.set_type(VResponse_Type_INPUT);
//    string out;
//    resp.SerializeToString(&out);
//    senddata.size = out.size();
//    senddata.buf = new unsigned char[senddata.size];
//    memcpy(senddata.buf,(out.c_str()),senddata.size);
//    client->sendData(&senddata);
}

void SendThread::requestOutput(int l,int m)
{
    zoson::Data senddata;
    VResponse resp;
    resp.set_type(VResponse_Type_STATE);
    string state_str;
    VReqDeconv *req = vstate.mutable_deconv();
    const VReqFeature *fet = vstate.mutable_map();
    req->set_i_layer(l);
    if(l<0&&fet->i_layer()>=0)
    {
        req->set_i_layer(fet->i_layer());
    }else{
        return;
    }
    req->set_i_map(m);
    req->set_do_deconv(true);
    vstate.SerializeToString(&state_str);
    resp.set_data(state_str.c_str(),state_str.size());
    string out;
    resp.SerializeToString(&out);
    senddata.load_data((unsigned char*)const_cast<char*>(out.c_str()),out.size());
    client->sendData(&senddata);
}

void SendThread::requestWeightDiff()
{
    zoson::Data senddata;
    VResponse resp;
    resp.set_type(VResponse_Type_STATE);
    string state_str;

    VReqWeight *req = vstate.mutable_weight();
    req->set_diff(true);
    vstate.SerializeToString(&state_str);
    resp.set_data(state_str.c_str(),state_str.size());
    string out;
    resp.SerializeToString(&out);
    senddata.load_data((unsigned char*)const_cast<char*>(out.c_str()),out.size());
    client->sendData(&senddata);
}

void SendThread::run()
{
    vstate.set_input(0);
    VReqWeight *weight = vstate.mutable_weight();
    weight->set_index(0);
    VReqDeconv *deconv = vstate.mutable_deconv();
    deconv->set_i_layer(0);
    deconv->set_i_map(0);
    deconv->set_do_deconv(true);
    VReqFeature *map = vstate.mutable_map();
    map->set_i_image(0);
    map->set_i_layer(0);

    zoson::Data senddata;
    VResponse resp;
    resp.set_type(VResponse_Type_STATE);
    string state_str;
    vstate.SerializeToString(&state_str);
    resp.set_data(state_str.c_str(),state_str.size());
    string out;
    resp.SerializeToString(&out);
    senddata.load_data((unsigned char*)const_cast<char*>(out.c_str()),out.size());
    client->sendData(&senddata);
    exec();
}

