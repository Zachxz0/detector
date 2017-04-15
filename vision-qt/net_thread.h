#ifndef NET_THREAD_H
#define NET_THREAD_H
#include <QThread>
#include "net.hpp"
#include "zoson.pb.h"
#include <google/protobuf/message.h>
class ITCMsg
{
public:
    ITCMsg(){}
    ITCMsg(const ITCMsg& cmsg){
        this->msg = cmsg.msg;
    }
    ~ITCMsg()
    {
        delete msg;
    }

    ::google::protobuf::Message *msg;
};

class SendThread: public QThread
{
    Q_OBJECT
public:
    SendThread(zoson::NetClient*);
    ~SendThread(){};
protected:
    void run();
private slots:
    void requestFeature(int i_layer,int i_image,bool isDiff);
    void requestLayerInfo(int);
    void requestWeight(int);
    void requestInput(int);
    void requestOutput(int,int);
    void requestWeightDiff();

private:
    zoson::NetClient*client;
    VState vstate;
};

class NetThread : public QThread
{
    Q_OBJECT
public:
    NetThread();
    NetThread(zoson::NetClient*);
    ~NetThread(){
        delete client;
        delete sendThread;
    }
    SendThread* getSendThread(){return sendThread;}
protected:
    void getLayerInfos();
    void handleResponse(const VResponse*);
    void run();

signals:
    void recWeightMap(ITCMsg*);
    void recLayerInfo(ITCMsg* data);
    void recDeconv(ITCMsg*);
    void recInput(ITCMsg*);
    void recOutput(ITCMsg* data);
    void recFeature(ITCMsg*data);
private:
    zoson::NetClient* client;
    SendThread *sendThread;



};

#endif // NET_THREAD_H
