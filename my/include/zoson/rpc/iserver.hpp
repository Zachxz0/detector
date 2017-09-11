#ifndef _I_SERVER_HPP_
#define _I_SERVER_HPP_

#include <rpc/iprotocol.hpp>

namespace linker
{

class IServer:public IProtocol
{
public:
	virtual void encap(unsigned char*,int size)=0;
	virtual void parse(unsigned char*,int size)=0;
	
};

}

#endif 