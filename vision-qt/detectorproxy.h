#ifndef DETECTORPROXY_H
#define DETECTORPROXY_H
#include <net_thread.h>
class DetectorProxy: public QThread
{
    Q_OBJECT
public:
    DetectorProxy();
    SendThread* getSendThread();
protected:
    void run();

private slots:
    void getFeatureMap(ITCMsg*);
    void getDeconv(ITCMsg*);
    void getInput(ITCMsg*);
    void getLayerInfo(ITCMsg*);
    void getOutput(ITCMsg*);
    void getWeightMap(ITCMsg*);

signals:
    void recWeightMap(int w,int h,int c,int n,unsigned char*);
    void recLayerInfo(ITCMsg* data);
    void recDeconv(int w,int h,int c,unsigned char*);
    void recInput(int w,int h,int c,unsigned char*);
    void recOutput(ITCMsg* data);
    void recFeature(int w,int h,int c,int n,unsigned char*);
private :
    NetThread *thread;

};

#endif // DETECTORPROXY_H
