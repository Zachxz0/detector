#ifndef _DETECTOR_H_
#define _DETECTOR_H_

#include <caffe/caffe.hpp>
#include <string>
#include <proto/zoson.pb.h>
namespace boost{
template<typename point>
class shared_ptr;
}

namespace google
{
namespace protobuf
{
class Message;
}
}

namespace cv
{
class Mat;
}

namespace caffe
{
template<typename dtype>
class InputLayer;
}

using namespace std;
using namespace caffe;

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

	int init();
	int deInit();
	bool ifTrain(){return isTrain;};
	//test phase
	int initForTest();
	void deInitForTest();
    boost::shared_ptr<DetectOutput> doDetect(cv::Mat input);
	//train phase
	void doTrain();
	int initForTrain();
	int deInitForTrain();
	bool load_data(cv::Mat input);

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
	//void onTranact(::google::protobuf::Message& data){};
	//void tranact(::google::protobuf::Message& data){};

	string getName(){return m_param.name();}
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
	vector<string> lr_able_layer_names;// default conv 
	//vector<int> lr_able_layer_index;// default conv 


	DetectorParameter m_param;
	string model_path;
	string weight_path;
	string solver_path;
	string snapshot_path;
	float threshold;

    InputLayer<float>* inputLayer;
	Blob<float>* outputResult;

	vector<Callback*> listeners;
	VState state; 
	bool isTrain;
	
	Blob<float> mean_blob;
};

}

#endif
