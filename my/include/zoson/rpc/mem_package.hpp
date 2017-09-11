#ifndef _MEM_PACKAGE_HPP_
#define _MEM_PACKAGE_HPP_

#include <rpc/ipackage.hpp> 
 
namespace linker
{

class MemPackage:public IPackage
{
public:
	MemPackage();
	virtual ~MemPackage();

	virtual void writeInt(int val) ;
	virtual void writeInts(const int* vals,int len) ;
	virtual void writeUInt(unsigned int val) ;
	virtual void writeUInts(const unsigned int* vals,int len) ;
	virtual void writeChar(char val) ;
	virtual void writeChars(const char* val,int len) ;
	virtual void writeUChar(unsigned char val) ;
	virtual void writeUChars(const unsigned char* val,int len) ;
	virtual void writeBool(bool val) ;
	virtual void writeBools(const bool* val,int len) ;
	virtual void writeLong(long val) ;
	virtual void writeLongs(const long* val,int len) ;
	virtual void writeULong(unsigned long val) ;
	virtual void writeULongs(const unsigned long* val,int len) ;
	virtual void writeFloat(float val) ;
	virtual void writeFloats(const float* val,int len) ;
	virtual void writeDouble(double val) ;
	virtual void writeDoubles(const double* val,int len) ;

	virtual int readInt(int &val)const;
	virtual int readInts(int** val,int &len)const ;
	virtual int readUInt(unsigned int &val)const ;
	virtual int readUInts(unsigned int** val,int &len)const ;
	virtual int readChar(char& val)const ;
	virtual int readChars(char** val,int &len)const ;
	virtual int readUChar(unsigned char& val)const ;
	virtual int readUChars(unsigned char** val,int& len)const ;
	virtual int readBool(bool &val)const ;
	virtual int readBools(bool** val,int &len)const ;
	virtual int readLong(long &val)const ;
	virtual int readLongs(long** val,int &len) const;
	virtual int readULong(unsigned long &val)const ;
	virtual int readULongs(unsigned long** val,int& len)const ;
	virtual int readFloat(float &val)const ;
	virtual int readFloats(float** val,int& len)const ;
	virtual int readDouble(double &val)const ;
	virtual int readDoubles(double **val,int& len)const ;

	const unsigned char* getConstData();
	unsigned char* getMutableData();
	int getSize();
	int getCapacity();
	void print();
	virtual int read(const unsigned char* mem,int size);
	virtual void write(unsigned char** mem,int& size);

	struct Header
	{
		int type;
		int size;
		int num;
	};
	struct MemState
	{
		int length;
		int capacity;
		int wptr;
		int rptr;
	};
protected:
	virtual void applyMoreMem(unsigned int needMem);
	void writeDataToMem(unsigned char* data,int type,int size,int num);
	int readHeader(struct Header& header)const;
	int readDataFromMem(unsigned char* data,int type,int size,int num)const;
	void saveState();
	void restoreState();
	void clearState(MemState &state);
protected:
	unsigned char* mem;
	MemState curr_state;
	MemState last_state;
};

}



#endif