#ifndef _I_FUNCTION_HPP_
#define _I_FUNCTION_HPP_

#include <string>
#include <rpc/iprotocol.hpp>

namespace linker
{
class IFunction:public IProtocol
{
public:
	// virtual void invoke(const std::string&,...);
	// virtual void onInvoke(const std::string&,...);
	virtual void encap(unsigned char*,int size)=0;
	virtual void parse(unsigned char*,int size)=0;
	virtual unsigned long getSize()=0;
protected:

private:

};

}

#endif 