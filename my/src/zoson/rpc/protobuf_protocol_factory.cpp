#include <rpc/protobuf_protocol_factory.hpp>
#include <rpc/protobuf_params.hpp>
#include <rpc/protobuf_function.hpp>
#include <rpc/protobuf_server.hpp>
#include <rpc/protobuf_service.hpp>
#include <rpc/protobuf_termius.hpp>

namespace linker
{

ProtobufProtocolFactory* ProtobufProtocolFactory::instance_ = NULL;

ProtobufProtocolFactory::ProtobufProtocolFactory()
{

}

ProtobufProtocolFactory::~ProtobufProtocolFactory()
{

}

IParams* ProtobufProtocolFactory::newParams(const std::string& meta)
{
	return new ProtobufParams(meta);
}

IFunction* ProtobufProtocolFactory::newFunction(const std::string& meta)
{
	return new ProtobufFunction(meta);
}

IService* ProtobufProtocolFactory::newService(const std::string& meta)
{
	return new ProtobufService(meta);
}

IServer* ProtobufProtocolFactory::newServer(const std::string&meta)
{
	return new ProtobufServer(meta);
}

ITermius* ProtobufProtocolFactory::newITermius(const std::string& meta)
{
	return new ProtobufTermius(meta);
}

ProtobufProtocolFactory* ProtobufProtocolFactory::getInstance()
{
	if(instance_ != NULL)
	{
		instance_ = new ProtobufProtocolFactory();
	}
	return instance_;
}

}