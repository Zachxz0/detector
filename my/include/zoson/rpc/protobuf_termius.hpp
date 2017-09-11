#ifndef _PROTOBUF_TERMIUS_HPP_
#define _PROTOBUF_TERMIUS_HPP_

#include <rpc/itermius.hpp>
#include <string>
namespace linker
{

class TermiusParams;

class ProtobufTermius:public ITermius
{
public:
	ProtobufTermius(const std::string&);
	ProtobufTermius(TermiusParams& params);
	virtual ~ProtobufTermius();
	virtual void encap(unsigned char*,int size){}
	virtual void parse(unsigned char*,int size){}
	virtual unsigned long getSize(){return 1;}
protected:

private:


};

}

#endif