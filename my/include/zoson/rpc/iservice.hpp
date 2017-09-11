#ifndef _I_SERVICE_HPP_
#define _I_SERVICE_HPP_

#include <rpc/iprotocol.hpp>

namespace linker
{

class IService:public IProtocol
{
public:
	// virtual bool isAlive()=0;
	// virtual void mapFuncPtr()=0;
	virtual void encap(unsigned char*,int size)=0;
	virtual void parse(unsigned char*,int size)=0;
protected:

private:
};

}

#endif