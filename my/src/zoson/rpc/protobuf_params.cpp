#include <rpc/protobuf_params.hpp>
#include <rpc/iparams.hpp>
#include <utils/common.hpp>
#include <rpc/protobuf_protocol_factory.hpp>
#include <proto/linker.pb.h>
#include <rpc/mem_package.hpp>
#include <iostream>

#define PACKDATA(AP,PACK,FUNC,TYPE,RTYPE,ARR) if(ARR) \
	{\
		TYPE * ptr = va_arg(AP,TYPE *);\
		int len = va_arg(AP,int);\
		PACK->FUNC##s(ptr,len);\
	}else{\
		TYPE val = (TYPE)va_arg(AP,RTYPE);\
		PACK->FUNC(val); \
	}

#define READDATA(AP,PACK,FUNC,TYPE,RTYPE,ARR) if(ARR) \
	{\
		TYPE ** ptr = va_arg(AP,TYPE **);\
		int* len = (int*)((va_arg(AP,int*)));\
		PACK->FUNC##s(ptr,*len);\
	}else{\
		TYPE * val = va_arg(AP,TYPE *);\
		PACK->FUNC(*val);\
	}

namespace linker  
{

ProtobufParams::ProtobufParams(const std::string& params_file):params_(NULL),pack_(NULL)
{
	ParamParams param; 
	zoson::read_proto_txt(&param,params_file); 
	initByParams(param); 
}

ProtobufParams::ProtobufParams(ParamParams& params):params_(NULL),pack_(NULL)
{
	initByParams(params);
}

ProtobufParams::~ProtobufParams()
{
	cout<<"delelte protobuf params"<<endl;
	if(params_!=NULL)delete params_;
	if(pack_!=NULL)delete pack_;
}

void ProtobufParams::initByParams(ParamParams& params)
{
	this->params_ = new ParamParams();
	this->params_->CopyFrom(params);
	initTypeIndex();
}

void ProtobufParams::encap(unsigned char* mem,int size)
{
	if(pack_!=NULL)
	{
		memcpy(mem,pack_->getConstData(),pack_->getSize());
		size = pack_->getSize();
	}else{
		size = -1;
	}
}

void ProtobufParams::parse(unsigned char* mem,int size)
{
	if(pack_==NULL)pack_ = new MemPackage();
	pack_->read(mem,size);
}

void ProtobufParams::initTypeIndex()
{
	if(params_!=NULL)
	{
		IPackage::TYPE_INT = DType::INT;
		IPackage::TYPE_UINT =  DType::UINT;
		IPackage::TYPE_CHAR =  DType::CHAR;
		IPackage::TYPE_UCHAR =  DType::UCHAR;
		IPackage::TYPE_BOOL =  DType::BOOL;
		IPackage::TYPE_LONG =  DType::LONG;
		IPackage::TYPE_ULONG =  DType::ULONG;
		IPackage::TYPE_FLOAT =  DType::FLOAT;
		IPackage::TYPE_DOUBLE =  DType::DOUBLE;
	}
}

unsigned long  ProtobufParams::getSize()
{
	return pack_->getSize();
}

void ProtobufParams::read(int flag,...)
{
	if(pack_==NULL||params_==NULL)return;
	va_list ap;
	va_start(ap,flag);
	int params_size = params_->data_size();
	for(int i=0;i<params_size;++i)
	{
		const DType& dt = params_->data(i);
		int type = dt.type();
		bool isArr = dt.arr();
		switch(type)
		{
			case DType::INT:
				READDATA(ap,pack_,readInt,int,int,isArr)
				break;
			case DType::UINT:
				READDATA(ap,pack_,readUInt,unsigned int,unsigned int,isArr)
			break;
			case DType::CHAR:
				READDATA(ap,pack_,readChar,char,int,isArr)
			break;
			case DType::UCHAR:
				READDATA(ap,pack_,readUChar,unsigned char,int,isArr)
			break;
			case DType::BOOL:
				READDATA(ap,pack_,readBool,bool,int,isArr)
			break;
			case DType::LONG:
				READDATA(ap,pack_,readLong,long,int,isArr)
			break;
			case DType::ULONG:
				READDATA(ap,pack_,readULong,unsigned long,int,isArr)
			break;
			case DType::FLOAT:
				READDATA(ap,pack_,readFloat,float,double,isArr)
			break;
			case DType::DOUBLE:
				READDATA(ap,pack_,readDouble,double,double,isArr)
			break;
		}
	}
}

void ProtobufParams::write(int flag,...)
{
	if(params_==NULL)return;
	va_list ap;
	va_start(ap,flag);
	int params_size = params_->data_size();
	if(pack_==NULL)
	{
		pack_ = new MemPackage();
	}
	for(int i=0;i<params_size;++i)
	{
		const DType& dt = params_->data(i);
		int type = dt.type();
		bool isArr = dt.arr();
		switch(type)
		{
			case DType::INT:
				PACKDATA(ap,pack_,writeInt,int,int,isArr)
				break;
			case DType::UINT:
				PACKDATA(ap,pack_,writeUInt,unsigned int,int,isArr)
			break;
			case DType::CHAR:
				PACKDATA(ap,pack_,writeChar,char,int,isArr)
			break;
			case DType::UCHAR:
				PACKDATA(ap,pack_,writeUChar,unsigned char,int,isArr)
			break;
			case DType::BOOL:
				PACKDATA(ap,pack_,writeBool,bool,int,isArr)
			break;
			case DType::LONG:
				PACKDATA(ap,pack_,writeLong,long,long,isArr)
			break;
			case DType::ULONG:
				PACKDATA(ap,pack_,writeULong,unsigned long,unsigned long,isArr)
			break;
			case DType::FLOAT:
				PACKDATA(ap,pack_,writeFloat,float,double,isArr)
			break;
			case DType::DOUBLE:
				PACKDATA(ap,pack_,writeDouble,double,double,isArr)
			break;
		}
	}

}


}
