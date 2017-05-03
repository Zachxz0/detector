#include <commu/commu.hpp>
#include <utils/common.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include "net/server.h"
namespace zoson
{

Communicator::Communicator(const CommuParameter &commu):isConn(false)
{
	this->m_param = commu;
	initByParam(commu);
}

Communicator::Communicator(const string& file_path):isConn(false)
{
	read_proto_txt(&m_param,file_path);
	initByParam((const CommuParameter)m_param);
}

void Communicator::connect()
{
	if(isConn)
	{
		cout<<"Communicator is already connect"<<endl;
		return;
	}
	isConn = true;
	boost::shared_ptr<boost::thread> thr_ptr(new boost::thread(onConnect,this));
	this->thr_ser_rec = thr_ptr;
	thr_ser_rec->join();
}

void Communicator::initByParam(const CommuParameter& param)
{
	//default TCP
	boost::shared_ptr<Server>ptr(new Server());
	m_server = ptr;
	m_server->setAddress(param.address());
	m_server->setPost(param.port());
	m_type = param.type();
	m_server->setCallback(this);
}

const char* get()
{
	return "Commu";
}

void Communicator::getClientMessage(int fd,const char *buf,int size)
{
	cout<<"getClientMessage ::"<<fd<<endl;
	cout<<"content::"<<buf<<endl;
}

void Communicator::hasClientConn(int fd)
{
	cout<<"hasClientConn client fd :: "<<fd<<endl;
	this->m_id.push_back(fd);
}

void Communicator::onConnect(Communicator* comm)
{
	comm->m_server->startServer();
}

void Communicator::onSend(Communicator* comm)
{
	
}


}