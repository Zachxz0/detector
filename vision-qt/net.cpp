//
// Created by zoson on 17-2-7.
//

#include <sys/socket.h>
#include <sys/ioctl.h>
#include "net.hpp"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include<fcntl.h>
#include <bits/socket.h>
using namespace std;
namespace zoson
{

NetClient::NetClient() { }
NetClient::~NetClient() { }
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

void NetClient::flush()
{
    unsigned char buf[4096];
    while(recv(mSockfd, buf, 4096,0)>0);
}

bool NetClient::recData(Data* data,int reqcount)
{
    if(mSockfd<0)return false;

    int size =0;
    if(data->buf == NULL){
        data->buf = new unsigned char[4096];
        size = 4096;
        data->size = 0;
        data->capacity = size;
    }else{
        size = data->capacity;
        data->size = 0;
    }
    int offset = 0;
    int res_count = size;
    int rec_count = -1;
    if(reqcount<data->capacity&&reqcount>0){
        res_count = reqcount;
    }
    memset(data->buf,0,data->capacity);

    while((rec_count = recv(mSockfd, data->buf+offset, res_count,0)) != -1)
    {
        if(rec_count==reqcount)
        {
            data->size += rec_count;
            break;
        }
        if(res_count==rec_count&&rec_count!=reqcount-data->size)
        {
            unsigned char* temp_buf = new unsigned char[size*2];
            memcpy(temp_buf,data->buf,size);
            delete []data->buf;
            data->capacity = size*2;
            data->buf = temp_buf;
            offset += rec_count;
            data->size += rec_count;
            size = size*2;
            res_count = size - offset;
            if(res_count>(reqcount-data->size))res_count = reqcount-data->size;
            rec_count = -1;
        }else if((data->size+rec_count)<reqcount){
            data->size += rec_count;
            res_count -= rec_count;
            if(res_count>(reqcount-data->size))res_count = reqcount-data->size;
            offset+=rec_count;
            rec_count==-1;
        }else{
            data->size += rec_count;
            break;
        }
    }
    return true;
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

    return 0;
}


}
