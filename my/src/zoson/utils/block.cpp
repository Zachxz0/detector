#include <utils/block.hpp>

namespace zoson
{

Block::Block():blob(NULL),count(NULL)
{
	//LOG(INFO)<<"Construction";
	allocMem(this->shape);
	//LOG(INFO)<<"Construction end";
}

Block::Block(const Block& block):blob(NULL),count(NULL)
{
	//LOG(INFO)<<"Copy Construction";
	shallowCopy(block);
	//LOG(INFO)<<"Copy Construction end";
}

Block::Block(const vector<int>& shape):blob(NULL),count(NULL)
{
	allocMem(shape);
}

Block::Block(const string& data):blob(NULL),count(NULL)
{
	loadData(data);
}

Block::Block(const unsigned char* data,const vector<int>& shape):blob(NULL),count(NULL)
{
	loadData(data,shape);
}

Block::Block(const unsigned char* data,int len):blob(NULL),count(NULL)
{
	loadData(data,len);
}

Block::~Block()
{
	//LOG(INFO)<<"De Construction";
	clean();
	//LOG(INFO)<<"De Construction end";
}

Block& Block::operator=(const Block& block)
{
	//LOG(INFO)<<"= Construction";
	shallowCopy(block);
	return *this;
	//LOG(INFO)<<"= Construction end";
}

Block& Block::operator=(const string& data)
{
	//LOG(INFO)<<"= Construction";
	loadData(data);
	return *this;
	//LOG(INFO)<<"= Construction end";
}

void Block::copyFrom(const Block& block)
{
	//LOG(INFO)<<"CopyFrom";
	deepCopy(block);
	//LOG(INFO)<<"CopyFrom end";
}

void Block::clean()
{
	//LOG(INFO)<<"Clean";
	shape.clear();
	if(count==NULL)return;
	if(*count==1)
	{
		deepClean();
	}else if(*count>1){
		shallowClean();
	}
	//LOG(INFO)<<"Clean end";
}

void Block::deepClean()
{
	//LOG(INFO)<<"deepClean";
	if(blob!=NULL)
	{
		//LOG(INFO)<<"del blob";
		delete[] blob;
	}
	blob = NULL;
	if(count!=NULL)
	{
		//LOG(INFO)<<"del count";
		delete count;
	}
	count = NULL;
	//LOG(INFO)<<"deepClean end";
}

void Block::shallowClean()
{
	//LOG(INFO)<<"shallowClean";
	if(count!=NULL)--(*count);
	blob = NULL;
	//LOG(INFO)<<"shallowClean end";
}

void Block::deepCopy(const Block& block)
{
	//LOG(INFO)<<"deepCopy";
	clean();
	int num = 0;
	copyMem(block);
	//LOG(INFO)<<"deepCopy end";
}

void Block::shallowCopy(const Block& block)
{
	//LOG(INFO)<<"shallowCopy";
	clean();
	++(*block.count);
	this->shape = block.shape;
	this->count = block.count;
	this->blob = block.blob;
	//LOG(INFO)<<"shallowCopy end";
}

int Block::allocMem(const vector<int> &shape)
{
	//LOG(INFO)<<"allocMem";
	clean();
	this->count = new int(1);
	this->shape = shape;
	int num = getCount();
	if(num==0)return 0;
	this->blob = new unsigned char[num];
	return num;
}

int Block::copyMem(const Block& block)
{
	//LOG(INFO)<<"copyMem";
	shape = block.shape;
	int num = allocMem(block.shape);
	memcpy(this->blob,block.blob,num);
	//LOG(INFO)<<"copyMem end";
}

int Block::getCount()
{
	int dim = shape.size();
	if(dim==0)return 0;
	int num = 1;
	for(int i=0;i<dim;++i)num*=this->shape[i];
	return num;
}

void Block::reShape(const vector<int>& shape)
{
	//LOG(INFO)<<"reShape";
	allocMem(shape);
	//LOG(INFO)<<"reShape end";
}

void Block::print()
{
	//LOG(INFO)<<"print";
	int num = getCount();
	char* cont = new char[num+1];
	cont[num] = '\0';
	memcpy(cont,blob,num);
	LOG(INFO)<<cont;
	//LOG(INFO)<<"print end";
	delete[] cont;
}

bool Block::loadData(const unsigned char* data,const vector<int>& shape)
{
	if(data==NULL)return false;
	if(!checkShape(shape))return false;
	allocMem(shape);
	memcpy(this->blob,data,getCount());
	return true;
}

bool Block::loadData(const unsigned char* data,int len)
{
	if(len<=0)return false;
	vector<int> shape;
	shape.push_back(len);
	return loadData(data,shape);
}

bool Block::loadData(const string& data)
{
	loadData((const unsigned char*)data.c_str(),data.size());
}

bool Block::checkShape(const vector<int>&shape)
{
	if(shape.size()==0)return false;
	int dim = shape.size();
	for(int i=0;i<dim;i++)
	{
		if(shape[i]<=0)return false;
	}
	return true;
}

}