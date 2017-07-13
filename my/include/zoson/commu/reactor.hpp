#ifndef _REACTOR_HPP_
#define _REACTOR_HPP_
#include <google/protobuf/message.h>
#include <vector>

//todo
namespace zoson
{

class Communicator;
class Reactor
{
public:
	Reactor(Communicator* commu);
	//Reactor(Reactor*);
	virtual ~Reactor();
	virtual void onTranact(::google::protobuf::Message& data);
	virtual void tranact(::google::protobuf::Message& data)=0;
	virtual void emptyReq()=0;
	virtual void hasReq()=0;
	// virtual Reactor* newProxy()=0;
	// virtual void closeProxy(Reactor* proxy)=0;
	//virtual Reactor* get()=0;
	//void setCommu(Communicator *commu){this->m_commu=commu;}
protected:
	Communicator* m_commu;
	//Reactor *m_stub;
	// class Stub:public Reactor
	// {
	// friend zoson::Communicator;
	// public:
	// 	virtual void onTranact(::google::protobuf::Message& data)=0;
	// 	virtual void tranact(::google::protobuf::Message& data)=0;
	// 	virtual Reactor* get()=0;
	// protected:
	// }
	// class Proxy:public Reactor
	// {
	// friend zoson::Communicator;
	// public:
	// 	virtual void onTranact(::google::protobuf::Message& data)=0;
	// 	virtual void tranact(::google::protobuf::Message& data)=0;
	// 	virtual Reactor* get()=0;
	// protected:
	// }
};

}

#endif