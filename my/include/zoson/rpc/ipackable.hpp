#ifndef _I_PACKABLE_HPP_
#define _I_PACKABLE_HPP_

namespace linker
{

class IPackage;
class IPackable
{
public:
	virtual void writeToPackage(IPackage* in)=0;
	virtual int readFromPackage(const IPackage* out)=0;
protected:

private:

};

class TestPackage:public IPackable
{
public:
	TestPackage();
	virtual void writeToPackage(IPackage* in);
	virtual int readFromPackage(const IPackage* out);
	bool isEqual(TestPackage& package);
protected:

private:
	int test_int;
	char test_char;
	char test_chars[10];
};

TestPackage::TestPackage()
{
	test_int = 10;
	test_char = 'a';
	for(int i=0;i<1;++i)
	{
		test_chars[i] = 'a' + i;
	}
}

bool TestPackage::isEqual(TestPackage& package)
{
	if(package.test_int!=test_int)return false;
	if(package.test_char!=test_char)return false;
	for(int i=0;i<10;i++)
	{
		if(package.test_chars[i]!=test_chars[i])return false;
	}
	return true;
}

void TestPackage::writeToPackage(IPackage* in)
{
	in->writeInt(test_int);
	in->writeChar(test_char);
	in->writeChars(test_chars,10);
}

int TestPackage::readFromPackage(const IPackage* out)
{
	if(out->readInt(test_int)==-1)return -1;
	if(out->readChar(test_char))return -1;
	char* cs;
	int len;
	if(out->readChars(&cs,len)==-1)return -1;
	if(len == 10)
	{
		for(int i=0;i<len;++i)
		{
			test_chars[i] = cs[i];
		}
	}
}

}



#endif