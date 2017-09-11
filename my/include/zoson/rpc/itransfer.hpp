#ifndef _I_TRANSFER_HPP_
#define _I_TRANSFER_HPP_

#include <proto/linker.pb.h>

class Itransfer
{
public:
	virtual int open(const Url& url)=0; 
	virtual int read(unsigned char* cont,int len)=0;
	virtual int write(unsigned char* cont,int len)=0;
	virtual void close()=0;
protected:

private:
};

#endif