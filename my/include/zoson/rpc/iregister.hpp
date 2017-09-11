#ifndef _I_REGISTER_HPP_
#define _I_REGISTER_HPP_

#include <proto/linker.pb.h>
#include <vector>
class IRegister:public IProtocol
{
public:
	virtual void addService(const Token& token)=0;
	virtual void rmService(const Token& token)=0;
	virtual IObject* getService(const Token& token)=0;
	virtual vector<Token> listServices()=0; 
protected:

private:
	
};

#endif