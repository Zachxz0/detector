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
	Reactor(){}
	virtual ~Reactor(){}
	virtual void onTranact(::google::protobuf::Message& data)=0;
	virtual void tranact(::google::protobuf::Message& data)=0;
	virtual Reactor* get()=0;
	class Stub:public Reactor
	{
	friend zoson::Communicator;
	public:
		virtual void onTranact(::google::protobuf::Message& data)=0;
		virtual void tranact(::google::protobuf::Message& data)=0;
		virtual Reactor* get()=0;
	protected:
	}
	class Proxy:public Reactor
	{
	friend zoson::Communicator;
	public:
		virtual void onTranact(::google::protobuf::Message& data)=0;
		virtual void tranact(::google::protobuf::Message& data)=0;
		virtual Reactor* get()=0;
	protected:
	}
};

}

#endif