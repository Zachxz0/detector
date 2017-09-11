#ifndef _PROTOBUF_PROTOCOL_FACTORY_HPP_
#define _PROTOBUF_PROTOCOL_FACTORY_HPP_

#include <rpc/iprotocol_factory.hpp>

namespace linker
{

class ProtobufProtocolFactory:public IProtocolFactory
{
public:
	ProtobufProtocolFactory* getInstance();
	virtual ~ProtobufProtocolFactory();
	virtual IParams* newParams(const std::string& meta);
	virtual IFunction* newFunction(const std::string& meta);
	virtual IService* newService(const std::string& meta);
	virtual IServer* newServer(const std::string&meta);
	virtual ITermius* newITermius(const std::string&);

	static ProtobufProtocolFactory *instance_;
protected:
	ProtobufProtocolFactory();
private:

};

}

#endif