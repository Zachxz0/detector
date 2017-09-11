#ifndef _I_ITERMIUS_HPP_
#define _I_ITERMIUS_HPP_

#include <rpc/iprotocol.hpp>

namespace linker
{

class ITermius:public IProtocol
{
public:
	virtual void encap(unsigned char*,int size)=0;
	virtual void parse(unsigned char*,int size)=0;
protected:

private:

};

}

#endif