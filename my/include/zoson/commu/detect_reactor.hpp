#ifndef _DETECT_REACTOR_HPP_
#define _DETECT_REACTOR_HPP_
#include <google/protobuf/message.h>
#include <commu/reactor.hpp>
#include <proto/zoson.pb.h>
#include <detector/detector.hpp>
#include <google/protobuf/text_format.h>
using namespace google::protobuf;
using namespace google::protobuf::io;
namespace zoson
{

class DetectReactor:public Reactor,public Detector::Callback
{
public:
	DetectReactor(Detector* detector,Communicator* commu);
	//DetectReactor(DetectReactor*);
	virtual ~DetectReactor();
	virtual void onTranact(::google::protobuf::Message& data);
	virtual void tranact(::google::protobuf::Message& data);
	void haveMesssage(boost::shared_ptr<VResponse>msg);
	void emptyReq();
	void hasReq();
	//virtual Reactor* newProxy();
	//virtual void closeProxy(Reactor* proxy);
protected:
	Detector* m_detector;
private:

};


}


#endif