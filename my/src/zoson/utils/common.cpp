#include <utils/common.hpp>
#include <float.h> 
namespace zoson
{

void read_proto_txt(Message *msg,const string file_path)
{
	ifstream in(file_path.c_str());
	string instr;
	char buf[256];
	while(!in.eof())
	{
		in.read(buf,256);
		instr.append(buf);
	}
	in.close();
	TextFormat::ParseFromString(instr,msg);
	in.close();
}

void write_proto_bin(const Message* msg , const string file_path)
{
	fstream output(file_path.c_str(),ios::out);
	msg->SerializeToOstream(&output);
	output.close();
}

void read_proto_bin(Message* msg, const string file_path)
{
	ifstream input(file_path.c_str());
	msg->ParseFromIstream(&input);
	input.close();
}

void write_proto_txt(const Message *msg,const string file_path)
{
	fstream strin(file_path.c_str(),ios::out);
	string msg_txt;
	TextFormat::PrintToString(*msg,&msg_txt);
	strin.write(msg_txt.c_str(),msg_txt.size());
	strin.close();
}

void findMaxMin(const float*in,int count,float&max,float&min)
{
	if(count<=0||in==NULL)return;
	max = FLT_MIN;
	min = FLT_MAX;
	for(int i=0;i<count;++i)
	{
		if(in[i]>max)max = in[i];
		if(in[i]<min)min = in[i];
	}

}

void findMin(const float* in,int count,float&min)
{
	if(count<=0||in==NULL)return;
	min = FLT_MAX;
	for(int i=0;i<count;++i)
	{
		if(in[i]<min)min = in[i];
	}
}

void findMax(const float* in,int count,float&max)
{
	if(count<=0||in==NULL)return;
	max = FLT_MIN;
	for(int i=0;i<count;++i)
	{
		if(in[i]>max)max = in[i];
	}
}

unsigned char* normTo255(const float* in,int count)
{
	if(count<=0||in==NULL)return NULL;
	unsigned char* out = new unsigned char[count];
	float max ,min;
	findMaxMin(in,count,max,min);
	max -= min;
	bool isZero = max<1e-6&&max>-(1e-6)&&min<1e-6&&min>-(1e-6)&&(max-min)<1e6;
	for(int i=0;i<count;++i)
	{
		if(isZero)
		{
			out[i] = 1;
		}else{
			unsigned char tmp = (unsigned char)(255*(in[i] - min)/(max+1e-10));
			if(tmp=='\0')tmp=1;
			out[i] = tmp;
		}
		
	}
	return out;
}

void normTo255(unsigned char* out,const float* in,int count)
{
	if(count<=0||in==NULL||out==NULL)return ;
	float max,min;
	findMaxMin(in,count,max,min);
	max -= min;
	bool isZero = max<1e-6&&max>-(1e-6)&&min<1e-6&&min>-(1e-6)&&(max-min)<1e6;
	for(int i=0;i<count;++i)
	{
		if(isZero)
		{
			out[i] = 1;
		}else{
			unsigned char tmp = (unsigned char)(255*(in[i] - min)/(max+1e-10));
			if(tmp=='\0')tmp=1;
			out[i] = tmp;
		}
	}
}

void addMean(unsigned char* udata,int channel,int wh,int mean[],int dim)
{
	if(mean==NULL)return;

	for(int i=0;i<wh;++i)
	{
		for(int j=0;j<channel;++j)
		{
			udata[i+j*wh] +=  mean[j];
			if(udata[i+j*wh] == 0)udata[i+j*wh] = 1;
		}
	}
}

}