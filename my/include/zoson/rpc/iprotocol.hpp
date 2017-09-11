#ifndef _I_PROTOCOL_HPP_
#define _I_PROTOCOL_HPP_
#include <string>

namespace linker
{

class IProtocol
{
public:
	virtual void encap(unsigned char*,int size)=0;
	virtual void parse(unsigned char*,int size)=0;
	virtual unsigned long getSize()=0;
protected:
	IProtocol *parent;
private:
	
};

class IProtocols:public IProtocol
{
public:
	virtual IProtocol* getSub(const std::string&)=0;
	virtual void addSub(const std::string&)=0;
	virtual void addSub(IProtocol*)=0;
	virtual void rmSub(IProtocol*)=0;
	virtual void rmSub(const std::string&)=0;
	virtual void rmAll()=0;
	virtual void setParent(IProtocol* par)=0;
};

}

#endif
