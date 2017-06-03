//
// Created by zoson on 17-2-7.
//

#include <sys/socket.h>
#include "net.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
using namespace std;
namespace zoson
{

NetClient::NetClient() 
{
    mdata.buf = new unsigned char[4096];
    mdata.size = 4096;
}

NetClient::~NetClient() 
{
    if(callback!=NULL)delete callback; 
}
bool NetClient::connectServer(ConnInfo connInfo)
{
    mConnInfo = connInfo;
    createConnection();
    return false;
}
bool NetClient::sendData(const Data* data)
{
    if(mSockfd<0)return false;
    unsigned char* buf = data->buf;
    int size = data->size;
    if( send(mSockfd, buf, size, 0) < 0)
    {
        return false;
    }
    return true;
}

void NetClient::loopForData()
{
    if(mSockfd<0)return false;
    int offset = 0;
    int res_count = data.size;
    int rec_count = -1;
    while((rec_count = recv(mSockfd, data->buf+offset, res_count,0)) != -1)
    {
        if(res_count==rec_count)
        {
            unsigned char* temp_buf = new unsigned char[size*2];
            memcpy(temp_buf,data->buf,size);
            delete []data->buf;
            data->buf = temp_buf;
            offset += rec_count;
            data->size += rec_count;
            size = size*2;
            res_count = size - offset;
            rec_count = -1;
        }else{
            data->size += rec_count;
            break;
        }
    }
    if(callback!=NULL)
    {
        callback->getServerMessage(data.buf,data.size);
    }
}

void NetClient::disConnect()
{
    close(mSockfd);
    mSockfd = -1;
}

int NetClient::createConnection()
{
    struct sockaddr_in    servaddr;
    if( (mSockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        return -1;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(mConnInfo.server_post);
    if( inet_pton(AF_INET, mConnInfo.server_addr.c_str(), &servaddr.sin_addr) <= 0){
        return -1;
    }
    if( connect(mSockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        return -1;
    }
    if(callback!=NULL)
    {
        callback->hasServerConn(mSockfd);
    }
    return 0;
}

void setCallback(ClientCallback* cb)
{
    this->callback = cb;
}

}
