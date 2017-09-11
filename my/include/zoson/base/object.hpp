#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <mutex>
#include <string>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <condition_variable>
using namespace std;
namespace zoson
{
//typedef shared_ptr<Object> Spoint;
//typedef weak_ptr<Object> Wpoint;
class Object
{
public:
	Object();
	virtual ~Object();
	virtual inline bool equals(const Object& obj){ return (this==(&obj)); }
	virtual string toString(){return "Object";};
	void notify();
	void notifyAll();
	void wait(long time);
	void wait();
	virtual string print(){return "Object";};
	virtual string onSerialize(){return "to do";}
	virtual void deSerialize(string){};
private:
	mutex m_mutex;
	condition_variable m_cv;
};

}

#endif