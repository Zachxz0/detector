#include <commu/commu.hpp>
#include <commu/reactor.hpp>
#include <utils/common.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include "net/tcp_server.h"
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
	//thr_ser_rec->join();
}

void Communicator::join()
{
	this->thr_ser_rec->join();
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
	int ser_size = m_service.size();
	for(int i=0;i<ser_size;++i)
	{
		boost::shared_ptr<Reactor> reactor = m_service[i];
		VResponse req ;
        string in(buf,size);
        req.ParseFromString(in);
		reactor->tranact(req);
	}

}

void Communicator::hasClientConn(int fd)
{
	this->m_id.push_back(fd);
	if(m_id.size()==1)
	{
		m_service[0]->hasReq();
	}
}

void Communicator::hasClientClose(int fd)
{
	vector<int>::iterator i;
	for(i=m_id.begin();i!=m_id.end();)
	{
		if(*i==fd)
		{
			m_id.erase(i);
			break;
		}
		i++;
	}
	if(m_id.size()==0)
	{
		m_service[0]->emptyReq();
	}
	//this->m_id.erase
}

void Communicator::onConnect(Communicator* comm)
{
	comm->m_server->startServer();
}

void Communicator::onSend(Communicator* comm)
{
	
}

void Communicator::addService(const string& name,boost::shared_ptr<Reactor> reactor)
{
	m_service.push_back(reactor);
}

void Communicator::sendMessage(::google::protobuf::Message& data)
{
	if(m_id.size()==0)
	{
		cout<<"Communicator ::sendMessage :no connector"<<endl;
		return;
	}
	boost::mutex::scoped_lock lock(io_mutex);
    string out;
    data.SerializeToString(&out);
    string count;
    Bytetranfer bytes;
    bytes.set_count(out.size());
    bytes.SerializeToString(&count);
    m_server->sendMessage(count.c_str(),count.size());
    usleep(1);
    m_server->sendMessage(out.c_str(),out.size());
}

}