#include <rpc/mem_package.hpp>
#include <cstring>
#include <iostream>
#define WRITEDATA(DATA,TYPE,TYPE_INTDEX) applyMoreMem(sizeof(TYPE));\
	writeDataToMem((unsigned char*)&DATA,TYPE_INTDEX,sizeof(TYPE),1);
#define WRITEDATAS(DATA,TYPE,LEN,TYPE_INTDEX) applyMoreMem(sizeof(TYPE)*LEN);\
	writeDataToMem((unsigned char*)DATA,TYPE_INTDEX,sizeof(TYPE),LEN);
#define READDATA(VAL,TYPE) Header header;\
	if(readHeader(header)==-1)return -1;\
	if(readDataFromMem((unsigned char*)&VAL,header.type,sizeof(TYPE),header.num)==-1)return -1;
#define READDATAS(VAL,TYPE,LEN) Header header;\
	if(readHeader(header)==-1)return -1;\
	*VAL = new TYPE[header.num];\
	if(readDataFromMem((unsigned char*)*VAL,header.type,sizeof(TYPE),header.num))return -1;\
	len = header.num;
 

namespace linker
{

MemPackage::MemPackage():mem(NULL)
{
	clearState(curr_state);
	clearState(last_state); 
}
 
MemPackage::~MemPackage()
{
	if(mem!=NULL)delete[] mem;
}

const unsigned char* MemPackage::getConstData()
{
	return mem;
}

unsigned char* MemPackage::getMutableData()
{
	return mem;
}

int MemPackage::getSize()
{
	return curr_state.length;
}

int MemPackage::getCapacity()
{
	return curr_state.capacity;
}

int MemPackage::read(const unsigned char*mem,int size)
{
	if(mem==NULL||size<=0)return -1;
	applyMoreMem(size);
	memcpy(this->mem,mem,size);
	curr_state.length = size;
	return 0;
}

void MemPackage::write(unsigned char** mem,int&size)
{
	if(this->mem==NULL||curr_state.length<=0)
	{
		size==-1;
		return;
	}else{
		size = curr_state.length;
		*mem = new unsigned char[size];
		memcpy(mem,this->mem,size);
	}
}

void MemPackage::applyMoreMem(unsigned int needMem)
{
	needMem += sizeof(Header);
	if(mem==NULL){
		int size = 64;
		while(size<needMem)
		{
			size *= 2;
		}
		this->curr_state.capacity = size;
		this->mem = new unsigned char[size];
	}else if(needMem>(curr_state.capacity-curr_state.length)){
		int size = curr_state.capacity*2;
		while((size- curr_state.length)<needMem)
		{
			size *= 2;
		}
		unsigned char* new_mem = new unsigned char[size];
		memcpy(new_mem,mem,curr_state.length);
		delete[] mem;
		mem = new_mem;
		this->curr_state.capacity = size;
	} 
}

void MemPackage::writeDataToMem(unsigned char* data,int type,int size,int num)
{
	struct Header header;
	header.type = type;
	header.size = size; 
	header.num = num;
	memcpy(mem+curr_state.wptr,&header,sizeof(Header));
	curr_state.wptr += sizeof(Header);
	memcpy(mem+curr_state.wptr,data,size*num);
	curr_state.wptr += size*num;
	curr_state.length = curr_state.wptr;
}

int MemPackage::readHeader(struct Header& header)const
{
	if(mem==NULL)return -1;
	if((curr_state.length-curr_state.rptr)<sizeof(Header))return -1;
	memcpy(&header,mem+curr_state.rptr,sizeof(Header));
	(const_cast<MemPackage*>(this))->curr_state.rptr += sizeof(Header);
	return 0;
}

int MemPackage::readDataFromMem(unsigned char* data,int type,int size,int num)const
{
	if((curr_state.length-curr_state.rptr)<(size*num))return -1;
	memcpy(data,mem+curr_state.rptr,size*num);
	(const_cast<MemPackage*>(this))->curr_state.rptr += size*num; 
	if(curr_state.rptr==curr_state.length)
	{
		delete[] (const_cast<MemPackage*>(this))->mem; 
		(const_cast<MemPackage*>(this))->mem = NULL;
	}
	return 0; 
} 
 
void MemPackage::clearState(MemState &state)
{
	state.length = 0;
	state.capacity = 0;
	state.wptr = 0;
	state.rptr = 0;
}

void MemPackage::saveState()
{
	this->last_state = this->curr_state;
}

void MemPackage::restoreState()
{
	this->curr_state = this->last_state;
}

void MemPackage::writeInt(int val) 
{
	WRITEDATA(val,int,IPackage::TYPE_INT)
}

void MemPackage::writeInts(const int* val,int len)
{
	WRITEDATAS(val,int,len,IPackage::TYPE_INT)
}

void MemPackage::writeUInt(unsigned int val) 
{
	WRITEDATA(val,unsigned int,IPackage::TYPE_UINT)
}

void MemPackage::writeUInts(const unsigned int* val,int len)
{
	WRITEDATAS(val,unsigned int,len,IPackage::TYPE_UINT)
}

void MemPackage::writeChar(char val)
{
	WRITEDATA(val,char,IPackage::TYPE_CHAR)
}

void MemPackage::writeChars(const char* val,int len)
{
	WRITEDATAS(val,char,len,IPackage::TYPE_CHAR)
}

void MemPackage::writeUChar(unsigned char val)
{
	WRITEDATA(val,unsigned char,IPackage::TYPE_UCHAR)
}

void MemPackage::writeUChars(const unsigned char* val,int len)
{
	WRITEDATAS(val,unsigned char,len,IPackage::TYPE_UCHAR)
}

void MemPackage::writeBool(bool val)
{
	WRITEDATA(val,bool,IPackage::TYPE_BOOL)
}

void MemPackage::writeBools(const bool* val,int len)
{
	WRITEDATAS(val,bool,len,IPackage::TYPE_BOOL)
}

void MemPackage::writeLong(long val)
{
	WRITEDATA(val,long,IPackage::TYPE_LONG)
}

void MemPackage::writeLongs(const long* val,int len)
{
	WRITEDATAS(val,long,len,IPackage::TYPE_LONG)
}

void MemPackage::writeULong(unsigned long val)
{
	WRITEDATA(val,unsigned long,IPackage::TYPE_ULONG)
}

void MemPackage::writeULongs(const unsigned long* val,int len)
{
	WRITEDATAS(val,unsigned long,len,IPackage::TYPE_ULONG)
}

void MemPackage::writeFloat(float val)
{
	WRITEDATA(val,float,IPackage::TYPE_FLOAT) 
}

void MemPackage::writeFloats(const float* val,int len) 
{
	WRITEDATAS(val,float,len,IPackage::TYPE_FLOAT)
}

void MemPackage::writeDouble(double val)
{
	WRITEDATA(val,double,IPackage::TYPE_DOUBLE)
}

void MemPackage::writeDoubles(const double* val,int len)
{
	WRITEDATAS(val,double,len,IPackage::TYPE_DOUBLE)
}

int MemPackage::readInt(int &val)const
{
	READDATA(val,int)
	return 0;
}

int MemPackage::readInts(int** val,int &len)const
{
	READDATAS(val,int,len)
	return 0;
} 

int MemPackage::readUInt(unsigned int &val)const
{
	READDATA(val,unsigned int)
	return 0;
}

int MemPackage::readUInts(unsigned int** val,int &len)const
{
	READDATAS(val,unsigned int,len)
	return 0;
}

int MemPackage::readChar(char& val) const
{
	READDATA(val,char)
	return 0;
}

int MemPackage::readChars(char** val,int &len) const
{
	READDATAS(val,char,len)
	return 0;
}

int MemPackage::readUChar(unsigned char& val)const
{
	READDATA(val,unsigned char)
	return 0;
} 

int MemPackage::readUChars(unsigned char** val,int& len)const
{
	READDATAS(val,unsigned char,len)
	return 0;
}

int MemPackage::readBool(bool &val)const
{
	READDATA(val,bool)
	return 0;
}

int MemPackage::readBools(bool** val,int &len)const
{
	READDATAS(val,bool,len)
	return 0;
}

int MemPackage::readLong(long &val)const
{
	READDATA(val,long)
	return 0;
}

int MemPackage::readLongs(long** val,int &len)const
{
	READDATAS(val,long,len)
	return 0;
}

int MemPackage::readULong(unsigned long &val)const
{
	READDATA(val,unsigned long)
	return 0;
}

int MemPackage::readULongs(unsigned long** val,int& len)const
{
	READDATAS(val,unsigned long,len)
	return 0;
}

int MemPackage::readFloat(float &val) const
{
	READDATA(val,float)
	return 0;
}

int MemPackage::readFloats(float** val,int& len)const
{
	READDATAS(val,float,len)
}

int MemPackage::readDouble(double &val)const
{
	READDATA(val,double)
	return 0;
}

int MemPackage::readDoubles(double **val,int& len)const
{
	READDATAS(val,double,len)
	return 0;
}



// int MemPackage::TYPE_INT = 0x1;
// int MemPackage::TYPE_UINT = 0x2;
// int MemPackage::TYPE_CHAR = 0x3;
// int MemPackage::TYPE_UCHAR = 0x4;
// int MemPackage::TYPE_BOOL = 0x5;
// int MemPackage::TYPE_LONG = 0x6;
// int MemPackage::TYPE_ULONG = 0x7;
// int MemPackage::TYPE_FLOAT = 0x8;
// int MemPackage::TYPE_DOUBLE = 0x9;
// int MemPackage::TYPE_STRING = 0xa;
// int MemPackage::TYPE_PACKABLE = 0xb;

}