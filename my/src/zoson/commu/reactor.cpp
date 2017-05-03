#include <commu/reactor.hpp>

namespace zoson
{

Reactor::Reactor()
{

}

Reactor::~Reactor()
{

}

void Reactor::onTranact(::google::protobuf::Message& data);

void Reactor::tranact(::google::protobuf::Message& data)
{

	this->m_commu->sendMessage(data);
}

}