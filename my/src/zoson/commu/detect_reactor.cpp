#include <commu/detect_reactor.hpp>

namespace zoson
{

DetectReactor::DetectReactor(Detector *detector,Communicator* commu):Reactor(commu),m_detector(detector)
{
	//m_detector->addListeners(this);
}

//DetectReactor::DetectReactor()

DetectReactor::~DetectReactor()
{
	//m_detector->rmListeners(this);
}

void DetectReactor::onTranact(::google::protobuf::Message& data)
{
	Reactor::onTranact(data);
}

void DetectReactor::tranact(::google::protobuf::Message& data)
{
	VResponse* req= (VResponse*)(&data);
	switch(req->type()){
	    case VResponse_Type_FEATUREMAP:
	    {
	        VReqFeature req_f;
	        req_f.ParseFromString(req->data());
	        boost::shared_ptr<VResponse> rep_ptr(new VResponse);
	        m_detector->getFeatureMap(req_f,rep_ptr.get());
	        onTranact(*rep_ptr);
	    }
	    break;
	    case VResponse_Type_LAYERINFOS:
	    {
	        boost::shared_ptr<VResponse> rep_ptr(new VResponse);
	        m_detector->getDeconvAbleLayers(rep_ptr.get());
	        onTranact(*rep_ptr);
	        //cout<<"VResponse_Type_LAYERINFOS "<<endl;
	        // string out;
	        // rep_ptr->SerializeToString(&out);
	        // string count;
	        // Bytetranfer bytes;
	        // bytes.set_count(out.size());
	        // bytes.SerializeToString(&count);
	        // cout<<"send:"<<bytes.count()<<endl;
	        // server->sendMessage(count.c_str(),count.size());
	        // usleep(1);
	        // server->sendMessage(out.c_str(),out.size());
	    }
	    break;
	    case VResponse_Type_WEIGHT:
	    {
	        boost::shared_ptr<VResponse> rep_ptr(new VResponse());
	        VReqWeight req_w ;
	        req_w.ParseFromString(req->data());
	        m_detector->getWeightForTest(req_w,rep_ptr.get());
	        onTranact(*rep_ptr);
	    }
	    break;
	    case VResponse_Type_INPUT:
	    {
	        boost::shared_ptr<VResponse> rep_ptr(new VResponse);
	        m_detector->getOriginalImage(0,rep_ptr.get());
	        onTranact(*rep_ptr);
	    }
	    break;
	    case VResponse_Type_STATE:
	    {
	        VState state ;
	        state.ParseFromString(req->data());
	        string msg;
	        TextFormat::PrintToString(state,&msg);
	        m_detector->setState(state);
	    }
	    break;
	    // case VResponse_Type_REC:
	    // {
	    //     boost::mutex::scoped_lock lock(rec_mutex);
	    //     VRecState vrec_state;
	    //     vrec_state.ParseFromString(req.data());
	    //     if(vrec_state.state()==0){
	    //         rec_state = 0;
	    //     }else{
	    //         rec_state = -1;
	    //     }
	    //     cout<<"VResponse_Type_REC "<<rec_state<<endl;
	    // }
	    // break;
	}
}

void DetectReactor::haveMesssage(boost::shared_ptr<VResponse>msg)
{
	onTranact(*msg);
}

void DetectReactor::emptyReq()
{
	m_detector->rmListeners(this);
}

void DetectReactor::hasReq()
{
	m_detector->addListeners(this);
}
// Reactor* DetectReactor::newProxy()
// {
// 	DetectReactor* proxy = new DetectReactor(this);
// 	m_detector->addListeners(proxy);
// 	return proxy;
// }

// void DetectReactor::closeProxy(Reactor* proxy)
// {
// 	m_detector->rmListeners(proxy);
// 	delete proxy;
// }

}