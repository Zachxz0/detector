#ifndef _I_PARAMS_HPP_
#define _I_PARAMS_HPP_
#include <rpc/iprotocol.hpp>

namespace linker
{

class IParams:public IProtocol
{
public:
	virtual void write(int flag,...)=0;
	virtual void encap(unsigned char*,int size)=0;
	virtual void parse(unsigned char*,int size)=0;
	virtual void read(int flag,...)=0; 
private:
	void setParent(IProtocol* par){}
};

}

#endif