#ifndef _PROTOBUF_SERVER_HPP_
#define _PROTOBUF_SERVER_HPP_

#include <rpc/iserver.hpp>

namespace linker
{

class IServer;
class ServerParams;

class ProtobufServer:public IServer
{
public:
	ProtobufServer(const std::string&);
	ProtobufServer(ServerParams &params);
	virtual ~ProtobufServer();
	virtual void encap(unsigned char*,int size){}
	virtual void parse(unsigned char*,int size){}
	virtual unsigned long getSize(){return 1;}
protected:

private:

};	

}

#endif
