#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_
#include <vector>
#include <string>
#include <glog/logging.h>
using namespace std;
namespace zoson
{

class Block
{
public:
	Block();
	Block(const string& data);
	Block(const unsigned char* data,const vector<int>& shape);
	Block(const unsigned char* data,int len);
	Block(const Block&);
	Block(const vector<int>& shape);
	virtual ~Block();
	bool loadData(const unsigned char* data,const vector<int>& shape);
	bool loadData(const unsigned char* data,int len);
	bool loadData(const string& data);
	Block& operator=(const Block&);
	Block& operator=(const string&);
	unsigned char* getData()const{return blob;}
	virtual void copyFrom(const Block&);
	vector<int> getShape()const{return shape;};
	int getCount();
	void reShape(const vector<int>& shape);
	void print();
protected:
	virtual void deepClean();
	virtual void shallowClean();
	virtual void deepCopy(const Block&);
	virtual void shallowCopy(const Block&);
private:
	void clean();
	int allocMem(const vector<int> &);
	int copyMem(const Block&);
	bool checkShape(const vector<int>&shape);
protected:
	unsigned char *blob;
	vector<int> shape;
private:
	int *count;
};

}

#endif