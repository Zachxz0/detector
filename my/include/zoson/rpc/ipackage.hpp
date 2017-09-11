#ifndef _I_PACK_HPP_
#define _I_PACK_HPP_

namespace linker
{

class IPackable;
class IPackage
{
public:
	friend class IPackable;
	virtual void writeInt(int val)=0;
	virtual void writeInts(const int* vals,int len)=0;
	virtual void writeUInt(unsigned int val)=0;
	virtual void writeUInts(const unsigned int* vals,int len)=0;
	virtual void writeChar(char val)=0;
	virtual void writeChars(const char* val,int len)=0;
	virtual void writeUChar(unsigned char val)=0;
	virtual void writeUChars(const unsigned char* val,int len)=0;
	virtual void writeBool(bool val)=0;
	virtual void writeBools(const bool* val,int len)=0;
	virtual void writeLong(long val)=0;
	virtual void writeLongs(const long* val,int len)=0;
	virtual void writeULong(unsigned long val)=0;
	virtual void writeULongs(const unsigned long* val,int len)=0;
	virtual void writeFloat(float val)=0;
	virtual void writeFloats(const float* val,int len)=0;
	virtual void writeDouble(double val)=0;
	virtual void writeDoubles(const double* val,int len)=0;

	virtual int readInt(int &val)const =0;
	virtual int readInts(int** val,int &len)const=0;
	virtual int readUInt(unsigned int &val)const=0;
	virtual int readUInts(unsigned int** val,int &len)const=0;
	virtual int readChar(char& val)const=0;
	virtual int readChars(char** val,int &len)const=0;
	virtual int readUChar(unsigned char& val)const=0;
	virtual int readUChars(unsigned char** val,int& len)const=0;
	virtual int readBool(bool &val)const=0;
	virtual int readBools(bool** val,int &len)const=0;
	virtual int readLong(long &val)const=0;
	virtual int readLongs(long** val,int &len)const=0;
	virtual int readULong(unsigned long &val)const=0;
	virtual int readULongs(unsigned long** val,int& len)const=0;
	virtual int readFloat(float &val)const=0;
	virtual int readFloats(float** val,int& len)const=0;
	virtual int readDouble(double &val)const=0;
	virtual int readDoubles(double **val,int& len)const=0;

public:
	static int TYPE_INT ;
	static int TYPE_UINT ;
	static int TYPE_CHAR ;
	static int TYPE_UCHAR ;
	static int TYPE_BOOL ;
	static int TYPE_LONG ;
	static int TYPE_ULONG ;
	static int TYPE_FLOAT ;
	static int TYPE_DOUBLE ;
	static int TYPE_STRING ;
	static int TYPE_PACKABLE ;

};


}



#endif