#ifndef _COMMU_H_
#define _COMMU_H_

#include <proto/zoson.pb.h>
#include <map>
#include <vector>
#include "net/server.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <commu/reactor.hpp>
using namespace std;
namespace zoson
{

class Communicator:public ServerCallback
{
public:
	Communicator(const CommuParameter& commu);
	Communicator(const string& file_path);
	~Communicator(){};
	void connect();
	void sendMessage(::google::protobuf::Message&);
	
	void addService(const string& name,shared_ptr<Reactor> reactor);
	shared_ptr<Reactor> getService(const string& name);
	//ServerCallback interface
	void getClientMessage(int fd,const char *buf,int size);
	void hasClientConn(int fd);
protected:
	void initByParam(const CommuParameter &param);
	Communicator(const Communicator&){}
	static void onConnect(Communicator*);
	const char* get();
private:
	boost::shared_ptr<Server> m_server;
	CommuParameter_Type m_type;
	CommuParameter m_param;

	map<string,boost::shared_ptr<Reactor> > m_server_map;
	vector<boost::shared_ptr<Reactor> >m_service;
	vector<int> m_id;
	boost::shared_ptr<boost::thread> thr_ser;
	bool isConn;
};

}

#endif