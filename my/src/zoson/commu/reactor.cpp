#include <commu/reactor.hpp>
#include <commu/commu.hpp>
namespace zoson
{

Reactor::Reactor(Communicator* commu):m_commu(commu)
{

}

// Reactor::Reactor(Reactor* stub):m_stub(stub)
// {

// }

Reactor::~Reactor()
{
}

void Reactor::onTranact(::google::protobuf::Message& data)
{
	m_commu->sendMessage(data);
}

}