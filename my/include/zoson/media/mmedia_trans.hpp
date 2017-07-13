#ifndef _MMEDIA_TRANS_HPP_
#define _MMEDIA_TRANS_HPP_
#include <proto/zoson.pb.h>
#include <string>
using namespace std;
namespace zoson
{ 

class CallbackForData
{
public:
	virtual void rawData(unsigned char* data,int len)=0;
};

class MMediaUtils
{
public:
	MMediaUtils(){}
	MMediaUtils(const MMediaParameter& param);
	MMediaUtils(const string& param);
	virtual ~MMediaUtils();
	virtual void init();
	virtual void deInit();
	void setCallback(CallbackForData* cb);
	virtual int trans();

protected:
	MMediaUtils(const MMediaUtils&){}
private:
	
    CallbackForData *mCallback;
};

}

#endif