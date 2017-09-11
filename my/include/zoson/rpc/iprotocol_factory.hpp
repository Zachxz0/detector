#ifndef _I_PROTOCOL_FACTORY_HPP_
#define _I_PROTOCOL_FACTORY_HPP_

#include <string>
namespace linker
{

class IParams;
class IFunction;
class IService;
class IServer;
class ITermius;

class IProtocolFactory
{
public:
	virtual IParams* newParams(const std::string& meta)=0;
	virtual IFunction* newFunction(const std::string& meta)=0;
	virtual IService* newService(const std::string& meta)=0;
	virtual IServer* newServer(const std::string&meta)=0;
	virtual ITermius* newITermius(const std::string&)=0;
protected:

private:
	
};

}


#endif