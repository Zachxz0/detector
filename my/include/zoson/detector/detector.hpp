#ifndef _DETECTOR_H_
#define _DETECTOR_H_

#include <string>
#include <proto/zoson.pb.h>
#include <caffe/caffe.hpp>
#include "caffe/layers/zoson_data_layer.hpp" 
#include <google/protobuf/message.h>
//#include "detector/idetector.hpp"
namespace boost{
template<typename point>
class shared_ptr;
}

using caffe::Net;
using namespace boost;
using namespace std;
using namespace cv;
using namespace caffe;
using caffe::Caffe;
using caffe::Layer;
using caffe::Blob;
using caffe::Solver;

namespace zoson
{

class Detector: public Solver<float>::Callback
{
public:
	class Callback
	{
	public:
		virtual void haveMesssage(boost::shared_ptr<VResponse>msg)=0;
	};
	Detector(const DetectorParameter& param);
	Detector(const string& param_file);
	virtual ~Detector();

	//test phase
	int initForTest();
	void deInitForTest();
    boost::shared_ptr<DetectOutput> doDetect(Mat input);
	//train phase
	void doTrain();
	int initForTrain();
	int deInitForTrain();

    void getFeatureMap(const VReqFeature& reqFeature,VResponse*);
	void getDeconvAbleLayers(VResponse*);
    void getOriginalImage(int i,VResponse*);
    void getDeconvImage(const VReqDeconv &reqDeconv,VResponse*);
	void getWeightForTest(const VReqWeight&,VResponse*);
	void setState(VState&);

	void addListeners(Callback* cb);
	void rmListeners(Callback*cb);
	//solver callback
	void on_start();
	void on_gradients_ready();
	//Reactor
	// void onTranact(::google::protobuf::Message& data){};
	// void tranact(::google::protobuf::Message& data){};
protected:
	Detector(const Detector&){};
	void initByParam(const DetectorParameter& param);
	void notifyListeners(boost::shared_ptr<VResponse> msg);
private:
	Solver<float> *m_solver;
	Net<float> *m_caffe_net;
	Net<float> *m_caffe_test_net;

	vector<string> deconv_able_layer_names;
    vector<boost::shared_ptr<Layer<float> > > deconv_able_layers;
	vector<string> deconv_able_layer_types;
	vector<int> deconv_able_layer_index;

	DetectorParameter m_param;
	string model_path;
	string weight_path;
	string solver_path;
	string snapshot_path;
	float threshold;

    ZosonDataLayer<float>* inputLayer;
	Blob<float>* outputResult;

	vector<Callback*> listeners;
	VState state; 
	bool isTrain;
	
};

}

#endif
