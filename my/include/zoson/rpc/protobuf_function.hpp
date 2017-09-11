#ifndef _PROTOBUF_FUNCTION_HPP_
#define _PROTOBUF_FUNCTION_HPP_

#include <rpc/ifunction.hpp>
#include <vector>

class FunctionParams;
namespace linker
{

class IParams;

class FunctionParams;

class ProtobufFunction:public IFunction
{
public:
	ProtobufFunction(const std::string&);
	ProtobufFunction(FunctionParams &param);
	virtual void encap(unsigned char*,int size){}
	virtual void parse(unsigned char*,int size){}
	virtual unsigned long getSize(){return 1;}
protected:

private:
	std::vector<IParams*> subs_;
	FunctionParams *params_;
};

}

#endif