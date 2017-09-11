#ifndef _PROTOBU_PARAMS_HPP_
#define _PROTOBU_PARAMS_HPP_

#include <rpc/iparams.hpp>
#include <rpc/mem_package.hpp>
#include <string>
#include <vector>
#include <base/object.hpp>
class ParamParams;

namespace linker
{

class ProtobufParams:public IParams,public zoson::Object
{
public:
	ProtobufParams(const std::string&);
	ProtobufParams(ParamParams& params);
	virtual ~ProtobufParams();
	virtual void initByParams(ParamParams& params);
	virtual void write(int flag,...);
	virtual void read(int flag,...);
	virtual void encap(unsigned char*,int size);
	virtual void parse(unsigned char*,int size);
	unsigned long  getSize();

protected:
	void initTypeIndex();
private: 
	ParamParams *params_;
	MemPackage* pack_;
	struct Header header_;
};

}

#endif