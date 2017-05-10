//
// Created by zoson on 17-2-7.
//

#ifndef VISION_NET_HPP_H
#define VISION_NET_HPP_H
#include<string.h>
#include <string>
#include <sys/socket.h>
using namespace std;

namespace zoson
{
class NetClient;

class Data
{
public:
    friend zoson::NetClient;
    Data(unsigned char* b,int s):buf(b),size(s),capacity(s){}
    Data():buf(NULL),size(0),capacity(0){}
    void load_data(unsigned char* data,int len)
    {
        if(capacity == 0)
        {
            buf = new unsigned char[len];
            capacity = len;
            size = len;
        }else if(capacity<len)
        {
            while(capacity<len)
            {
                capacity *=capacity;
            }
            delete []buf;
            buf = new unsigned char[capacity];
        }
        memcpy(buf,data,len);
        this->size = len;
    }

    ~Data(){delete[] buf;}
    unsigned char * buf;
    int size;
    int capacity;
};

struct ConnInfo
{
    string server_addr;
    int server_post;
    string cookies;
};

class NetClient
{
public:
    NetClient();
    ~NetClient();
    bool connectServer(ConnInfo connInfo);
    bool sendData(const Data *data);
    bool recData(Data *data,int req = -1);
    string getTag(){return "NetClient";}
    void disConnect();
    void flush();
protected:
    int createConnection();
private:
    ConnInfo mConnInfo;
    int mSockfd;
};



}


#endif //VISION_NET_HPP_H
