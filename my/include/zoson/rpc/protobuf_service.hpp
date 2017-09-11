#ifndef _PROTOBUF_SERVICE_HPP_
#define _PROTOBUF_SERVICE_HPP_

#include <rpc/iservice.hpp>

namespace linker
{

class IService;
class ServiceParams;

class ProtobufService:public IService
{
public:
	ProtobufService(const std::string&);
	ProtobufService(ServiceParams &params);
	virtual ~ProtobufService();
	virtual void encap(unsigned char*,int size){}
	virtual void parse(unsigned char*,int size){}
	virtual unsigned long getSize(){return 1;}
protected:

private:

};

}


#endif