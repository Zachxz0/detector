#ifndef _COMMON_H_
#define _COMMON_H_
#include <string>
#include <google/protobuf/message.h>
#include <fstream>
#include <google/protobuf/text_format.h>
using namespace google::protobuf;
using namespace google::protobuf::io;
using namespace std;

namespace zoson
{

void read_proto_txt(Message *msg,string file_path);

void write_proto_bin(const Message* msg , string file_path);

void read_proto_bin(Message* msg, string file_path);

void write_proto_txt(const Message *msg,string file_path);

unsigned char* normTo255(const float* in,int count);

void normTo255(unsigned char* out,const float* in,int count);

//float* norm01(const float* in,int count);

void findMaxMin(const float*in,int count,float&max,float&min);

void findMin(const float* in,int count,float&min);

void findMax(const float* in,int count,float&max);

void addMean(unsigned char *,int count,int wh,int[],int);
}

#endif