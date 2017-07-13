#include <detector/detector.hpp>
#include <iostream>
#include <string>
#include <google/protobuf/message.h>
#include <utils/common.hpp>
#include "caffe/caffe.hpp"
#include "caffe/util/signal_handler.h"
#include "boost/algorithm/string.hpp"
#include <glog/logging.h>
#include <google/protobuf/text_format.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "caffe/util/io.hpp"
#include "caffe/layers/input_layer.hpp"
using namespace cv;
using namespace google::protobuf;
using namespace google::protobuf::io;
using namespace std;
using namespace boost;
using namespace caffe;

using namespace std;
namespace zoson
{

Detector::Detector(const DetectorParameter& param):
	m_caffe_net(NULL),m_param(param),inputLayer(NULL),outputResult(NULL)
{
	initByParam(param);
}

Detector::Detector(const string& param_file):
	m_caffe_net(NULL),inputLayer(NULL),outputResult(NULL)
{
	DetectorParameter param;
	read_proto_txt(&param,param_file);
	this->m_param = param;
	initByParam((const DetectorParameter)param);
}

void Detector::initByParam(const DetectorParameter &param)
{
	isTrain = false;
	model_path = param.model();
	weight_path = param.weight();
	snapshot_path = param.snapshot();
	if(param.istrain())
	{
		isTrain = true;
		solver_path = param.solver();
	}
}

Detector::~Detector()
{
	if(m_caffe_net!=NULL)delete m_caffe_net;
}

int Detector::init()
{
	if(isTrain)
	{
		initForTrain();
	}else{
		initForTest();
	}
}

int Detector::initForTest()
{
	isTrain = false;
	if(m_param.has_mean_file())
	{
		BlobProto blob_proto;
		//cout<<"mean : "<<m_param.mean_file().c_str()<<endl;
	    ReadProtoFromBinaryFileOrDie(m_param.mean_file().c_str(), &blob_proto);
	    mean_blob.FromProto(blob_proto);
	}
	if (m_param.use_gpu().size()>0 ) {
		int gpu_size = m_param.use_gpu().size();
		for(int i=0;i<gpu_size;++i)
		{
			Caffe::SetDevice(m_param.use_gpu(i));
		}
		Caffe::set_mode(Caffe::GPU);
	} else {
		Caffe::set_mode(Caffe::CPU);
	}
	m_caffe_net = new Net<float>(model_path,caffe::TEST,0,NULL);
	m_caffe_net->CopyTrainedLayersFrom(weight_path);
	outputResult = m_caffe_net->blob_by_name(m_param.out_name()).get();
//copy from initFortrain
	const vector<shared_ptr<Layer<float> > >& layers = m_caffe_net->layers();
	const vector<string> & layer_names = m_caffe_net->layer_names();
	inputLayer = (InputLayer<float>*)layers[0].get();//"data"
	vector<bool>& layer_need_backward = const_cast<vector<bool>& >(m_caffe_net->layer_need_backward());
	for(int i=0;i<layers.size();++i)
	{
		const char* type = layers[i]->type();
		if(strcmp(type,"Convolution")==0||strcmp(type,"Pooling")==0||strcmp(type,"Normalize")==0||strcmp(type,"Bn")==0||strcmp(type,"Deconvolution")==0||strcmp(type,"Unpooling")==0){
			deconv_able_layers.push_back(layers[i]);
			deconv_able_layer_names.push_back(layer_names[i]);
			deconv_able_layer_types.push_back(type);
			deconv_able_layer_index.push_back(i);
			if(strcmp(type,"Convolution")==0||strcmp(type,"Deconvolution")==0)
			{
				lr_able_layer_names.push_back(layer_names[i]);
			}
		}
	}
}

void Detector::deInitForTest()
{
	delete m_caffe_net;
	m_caffe_net = NULL;
}

bool Detector::load_data(Mat input)
{
	if(m_caffe_net->has_blob(m_param.in_name()))
	{
		//cout<<"has data blob"<<endl;
	}else{
		//cout<<"has no data blob"<<endl;
		return false;
	}
	Blob<float> *data = m_caffe_net->blobs()[0].get();
	bool has_mean_file = m_param.has_mean_file();
	bool has_mean = m_param.has_mean();
	if(has_mean_file)
	{
		if(data->channels()==mean_blob.channels()&&data->height()==mean_blob.height()&&data->width()==mean_blob.width())
		{
			has_mean_file = true;
		}else{
			has_mean_file = false;
		}
	}else if(has_mean){
		if(m_param.mean().value().size()==data->channels())
		{
			has_mean = true;
		}else{
			has_mean = false;
		}
	}
	int channels = data->channels();
	int height = data->height();
	int width = data->width();
	cv::Mat scale_mat;
    cv::resize(input, scale_mat, cv::Size(width, height ),(0, 0), (0, 0), cv::INTER_LINEAR);
	int pixel_index = 0;
	unsigned char *input_data = scale_mat.data;
	float* data_cpu = (data->mutable_cpu_data());
	const float*mean_data = NULL;
	if(m_param.has_mean_file())mean_data = mean_blob.cpu_data();
	for(int h=0;h<height;++h)
	{
		for(int w=0;w<width;++w)
		{
			for(int c=0;c<channels;++c)
			{
				float mean_value = 0;
				if(has_mean_file&&mean_data!=NULL)
				{
					mean_value = mean_data[c*width*height+h*height+w];
				}else if(has_mean)
				{
					mean_value = m_param.mean().value(c);
				}
				data_cpu[c*width*height+h*height+w] = (scale_mat.data[pixel_index++]- mean_value)*m_param.mean().scale();
			}
		}
	}
	return true;
}

float* Detector::rec_data(const float *data,int width,int height,int channels)
{
	if(data==NULL)return NULL;
	float* rec_data = new float[width*height*channels];
	bool has_mean_file = m_param.has_mean_file();
	bool has_mean = m_param.has_mean();
	const float*mean_data = NULL;
	if(m_param.has_mean_file())mean_data = mean_blob.cpu_data();
	if(has_mean_file)
	{
		if(channels==mean_blob.channels()&&height==mean_blob.height()&&width==mean_blob.width())
		{
			has_mean_file = true;
		}else{
			has_mean_file = false;
		}
	}else if(has_mean){
		if(m_param.mean().value().size()==channels)
		{
			has_mean = true;
		}else{
			has_mean = false;
		}
	}
	for(int h=0;h<height;++h)
	{
		for(int w=0;w<width;++w)
		{
			for(int c=0;c<channels;++c)
			{
				float mean_value = 0;
				if(has_mean_file&&mean_data!=NULL)
				{
					mean_value = mean_data[c*width*height+h*height+w];
				}else if(has_mean)
				{
					mean_value = m_param.mean().value(c);
				}
				//cout<<mean_value<<endl;
				rec_data[c*width*height+h*height+w] = data[c*width*height+h*height+w]/m_param.mean().scale() + mean_value;
			}
		}
	}
	return rec_data;
}

shared_ptr<DetectOutput> Detector::doDetect(Mat input)
{
	shared_ptr<DetectOutput> ptr(new DetectOutput());
	float iter_loss;
	load_data(input);
	m_caffe_net->Forward(&iter_loss);//(&iter_loss);
	if(outputResult==NULL)return ptr;
	//for SSD
	const float* result = outputResult->cpu_data();
	const int num_det = outputResult->height();
	if(num_det==0){
		cerr<<"detect failed"<<endl;
		return ptr;
	}	
	//cout<<"nm::"<<outputResult->num()<<" channel:"<<outputResult->channels()<<" width::"<<outputResult->width()<<" height:"<<outputResult->height()<<endl; 
	if(m_param.type()==DetectorParameter_Type_DETECTOR)
	{
		int width = input.rows;
		int height = input.cols;
		for(int i=0;i<num_det;++i){
			if(result[i*7+2]<0.5f)continue;
			float score = result[i*7+2];
			float label = result[i*7+1];
			float xmin = result[i*7+3]*width;
			float ymin = result[i*7+4]*height;
			float xmax = result[i*7+5]*width;
			float ymax = result[i*7+6]*height;

			Result *res = ptr->add_results();
			res->set_xmax(xmax);
			res->set_xmin(xmin);
			res->set_ymax(ymax);
			res->set_ymin(ymin);
			res->set_score(score);
			res->set_clazz(label);
		}
	}else if(m_param.type()==DetectorParameter_Type_CLASSIFIER)
	{
		int count = outputResult->count();
		float score  = 0.0f;
		int index =-1;
		for(int i=0;i<count;++i)
		{
			if(result[i]>score)
			{
				index = i;
				score = result[i];
			}
		}
		Result *res = ptr->add_results();
		res->set_xmax(0);
		res->set_xmin(0);
		res->set_ymax(0);
		res->set_ymin(0);
		res->set_score(score);
		res->set_clazz(index);
	}
	
	return ptr;
}

void CopyLayers(caffe::Solver<float>* solver, const std::string& model_list) {
  std::vector<std::string> model_names;
  boost::split(model_names, model_list, boost::is_any_of(",") );
  for (int i = 0; i < model_names.size(); ++i) {
    LOG(INFO) << "Finetuning from " << model_names[i];
    solver->net()->CopyTrainedLayersFrom(model_names[i]);
    for (int j = 0; j < solver->test_nets().size(); ++j) {
      solver->test_nets()[j]->CopyTrainedLayersFrom(model_names[i]);
    }
  }
}

int Detector::initForTrain()
{
	isTrain = true; 
	caffe::SolverParameter solver_param;
	caffe::ReadSolverParamsFromTextFileOrDie(solver_path, &solver_param);

	solver_param.mutable_train_state()->set_level(0);
	
	vector<int> gpus;
	gpus.push_back(0);
	if (gpus.size() == 0) {
		Caffe::set_mode(Caffe::CPU);
	} else {
		ostringstream s;
		for (int i = 0; i < gpus.size(); ++i) {
		  s << (i ? ", " : "") << gpus[i];
		}		
		solver_param.set_device_id(gpus[0]);
		Caffe::SetDevice(gpus[0]);
		Caffe::set_mode(Caffe::GPU);
		Caffe::set_solver_count(gpus.size());
	}

	caffe::SignalHandler signal_handler(
        caffe::SolverAction::STOP,
        caffe::SolverAction::SNAPSHOT);

	m_solver = caffe::SolverRegistry<float>::CreateSolver(solver_param);

	m_solver->SetActionFunction(signal_handler.GetActionFunction());

	if (snapshot_path.size()) {
		m_solver->Restore(snapshot_path.c_str());
	} else if (weight_path.size()!=0) {
		CopyLayers(m_solver, weight_path);
	}

	m_solver->add_callback(this);
	m_caffe_net = m_solver->net().get();
	const vector<shared_ptr<Layer<float> > >& layers = m_caffe_net->layers();
	const vector<string> & layer_names = m_caffe_net->layer_names();
	for(int i=0;i<layers.size();++i)
	{
		const char* type = layers[i]->type();
		if(strcmp(type,"Convolution")==0||strcmp(type,"Pooling")==0||strcmp(type,"Normalize")==0||strcmp(type,"Bn")==0||strcmp(type,"Deconvolution")==0||strcmp(type,"Unpooling")==0){
			deconv_able_layers.push_back(layers[i]);
			deconv_able_layer_names.push_back(layer_names[i]);
			deconv_able_layer_types.push_back(type);
			deconv_able_layer_index.push_back(i);
			if(strcmp(type,"Convolution")==0||strcmp(type,"Deconvolution")==0)
			{
				lr_able_layer_names.push_back(layer_names[i]);
			}
		}
	}
	shared_ptr<Solver<float> > solver_ptr(m_solver);
	if (gpus.size() > 1) {
		caffe::P2PSync<float> sync(solver_ptr, NULL, solver_ptr->param());
		sync.Run(gpus);
	} else {
		solver_ptr->Solve();
	}
	return 0;
}

int Detector::deInit()
{

}

int Detector::deInitForTrain()
{

}

void Detector::doTrain()
{

} 


void Detector::getFeatureMap(const VReqFeature& reqFeature,VResponse* rep_ptr)
{
	if(rep_ptr==NULL)
	{
		cerr<<"getFeatureMap VResponse point is null"<<endl;
		return ;
	}
	int i_layer = reqFeature.i_layer(); 
	int i_num = reqFeature.i_image();
	bool isDiff = reqFeature.diff();
	int layer_id = deconv_able_layer_index[i_layer];
	//int blob_id = m_caffe_net->top_ids(layer_id)[0];
	shared_ptr<Blob<float> >top = m_caffe_net->blob_by_name(deconv_able_layer_names[i_layer]);
	int w = top->width();
	int h = top->height();
	int c = top->channels();
	int size = w*h*c;
	int offset = i_num*size;
	string features(size,1);
	const float* fl_top ;
	if(!isDiff){
		fl_top = top->cpu_data();
	}else{
		fl_top = top->cpu_diff();
	}
	if(w==1&&h==1)
	{
		normTo255((unsigned char*)const_cast<char*>(features.data()),fl_top+offset,w*h*c);
	}else{
		for(int i=0;i<c;++i)
		{
			normTo255((unsigned char*)const_cast<char*>(features.data()+i*w*h),fl_top+offset+i*w*h,w*h);
		}
	}
	VFeatureMap map;
	map.set_width(w);
	map.set_height(h);
	map.set_channel(c);
	map.set_num(1);
	map.set_data(features);
	rep_ptr->set_type(VResponse_Type_FEATUREMAP);
	string map_str;
	map.SerializeToString(&map_str);
	rep_ptr->set_data(map_str);
}

void Detector::getDeconvAbleLayers(VResponse* rep_ptr)
{
	if(rep_ptr==NULL)
	{
		cerr<<"getDeconvAbleLayers VResponse point is null"<<endl;
		return ;
	}
	int num = deconv_able_layers.size();
	VLayerInfos *infos = new VLayerInfos();
	for(int i=0;i<num;++i)
	{
		VLayerInfo* info = infos->add_infos();;
		info->set_index(i);
		info->set_name(deconv_able_layer_names[i]);
		info->set_type(deconv_able_layer_types[i]);
	}
	string infos_str;
	TextFormat::PrintToString(*infos,&infos_str);
	rep_ptr->set_type(VResponse_Type_LAYERINFOS);
	rep_ptr->set_data(infos_str);
}

void Detector::getOriginalImage(int i,VResponse* rep_ptr)
{
	if(rep_ptr==NULL)
	{
		cerr<<"getOriginalImage VResponse point is null"<<endl;
		return ;
	}
	Blob<float> *data = m_caffe_net->blobs()[0].get();
	int w = data->width();
	int h = data->height();
	int c = data->channels();
	int size = w*h*c;
	string features(size,1);
	const float* fl_data = data->cpu_data();    
	float* recdata = rec_data(fl_data,w,h,c);
	normTo255((unsigned char*)const_cast<char*>(features.data()),fl_data,size);
	//unsigned char *ret_data = (unsigned char*)const_cast<char*>(features.data());
	VImage map;
	map.set_width(w);
	map.set_height(h);
	map.set_channel(c);
	map.set_data(features);
	rep_ptr->set_type(VResponse_Type_INPUT);
	string map_str;
	map.SerializeToString(&map_str);
	rep_ptr->set_data(map_str);
	//delete recdata;
}

void Detector::getDeconvImage(const VReqDeconv &reqDeconv,VResponse* rep_ptr)
{
	if(rep_ptr==NULL)
	{
		cerr<<"getDeconvImage VResponse point is null"<<endl;
		return ;
	}
	if(isTrain)
	{
		int i_layer = reqDeconv.i_layer();
		int i_map = reqDeconv.i_map();
		int layer_id = deconv_able_layer_index[i_layer];
		//int blob_id = m_caffe_net->top_ids(layer_id)[0];
		shared_ptr<Blob<float> >top = m_caffe_net->blob_by_name(deconv_able_layer_names[i_layer]);
		int w = top->width();
		int h = top->height();
		int c = top->channels();
		int size = w*h;
		string features(size,1);
		const float* fl_top ;
		fl_top = top->cpu_diff();
		normTo255((unsigned char*)const_cast<char*>(features.data()),fl_top+i_map*w*h,w*h);
		string rgbfeatures = features+features+features;
		VImage map;
		map.set_width(w);
		map.set_height(h);
		map.set_channel(1);
		map.set_data(rgbfeatures);
		rep_ptr->set_type(VResponse_Type_DECONV);
		string map_str;
		map.SerializeToString(&map_str);
		rep_ptr->set_data(map_str);
	}else{
		int i_layer = reqDeconv.i_layer();
		int i_map = reqDeconv.i_map();
		shared_ptr<Blob<float> > blob = m_caffe_net->blob_by_name(deconv_able_layer_names[i_layer]);
		float* diff = blob->mutable_cpu_diff();
		for(int i=0;i<blob->count();++i)
		{
			diff[i] = 0.0f;
		}
		float *dat = blob->mutable_cpu_data();
		for(int i=0;i<blob->width()*blob->height();++i)
		{
			diff[i_map*blob->width()*blob->height()+i] = dat[i_map*blob->width()*blob->height()+i];
		}

		int start_layer = deconv_able_layer_index[i_layer];
		const vector<shared_ptr <Blob<float> > > & blobs  = m_caffe_net->blobs();
		const vector<string> & blobs_name  = m_caffe_net->blob_names();
		int past = false;

		for(int i=0;i<blobs.size();++i)
		{
			if(past)
			{
				float* up_diff =  blobs[i]->mutable_cpu_diff();
				for(int j=0;j<blobs[i]->count();++j)
				{
					up_diff[i] = 0.0f;
				}
			}
			if(blobs_name[i]==deconv_able_layer_names[i_layer]){
				past = true;
			}
		}
		//m_caffe_net->Deconv();
		m_caffe_net->DeconvFromTo(start_layer,0);
		//copy code
		if(reqDeconv.do_sub_deconv())
		{
			int i_layer = reqDeconv.sub_i_layer();
			int i_map = reqDeconv.sub_i_map();
			shared_ptr<Blob<float> > blob = m_caffe_net->blob_by_name(deconv_able_layer_names[i_layer]);
			float* diff = blob->mutable_cpu_diff();
			for(int i=0;i<blob->count();++i)
			{
				if(i>=i_map*blob->width()*blob->height()&&i<(i_map+1)*blob->width()*blob->height())continue;
				diff[i] = 0.0f;
			}
			//float *dat = blob->mutable_cpu_data();
			// for(int i=0;i<blob->width()*blob->height();++i)
			// {
			// 	diff[i_map*blob->width()*blob->height()+i] = dat[i_map*blob->width()*blob->height()+i];
			// }
			int start_layer = deconv_able_layer_index[i_layer];
			const vector<shared_ptr <Blob<float> > > & blobs  = m_caffe_net->blobs();
			const vector<string> & blobs_name  = m_caffe_net->blob_names();
			int past = false;

			for(int i=0;i<blobs.size();++i)
			{
				if(past)
				{
					float* up_diff =  blobs[i]->mutable_cpu_diff();
					for(int j=0;j<blobs[i]->count();++j)
					{
						up_diff[i] = 0.0f;
					}
				}
				if(blobs_name[i]==deconv_able_layer_names[i_layer]){
					past = true;
				}
			}
			m_caffe_net->DeconvFromTo(start_layer,0);
		}

		Blob<float> *data = m_caffe_net->blobs()[0].get();
		int w = data->width();
		int h = data->height();
		int c = data->channels();
		int size = w*h*c;
		string features(size,1);
		const float* fl_data = data->cpu_diff();
		normTo255((unsigned char*)const_cast<char*>(features.data()),fl_data,size);

		VImage map;
		map.set_width(w);
		map.set_height(h);
		map.set_channel(c);
		map.set_data(features);
		rep_ptr->set_type(VResponse_Type_DECONV);
		string map_str;
		map.SerializeToString(&map_str);
		rep_ptr->set_data(map_str);
	}
	
}

void Detector::on_start()
{
	//notifyListeners(getDeconvAbleLayers());
}

void Detector::on_gradients_ready()
{
	if(listeners.size()==0)return;
	shared_ptr<VAll> all_ptr(new VAll());
	if(state.has_input()&&state.input()>=0)getOriginalImage(state.input(),all_ptr->add_response());
	if(state.has_deconv()&&state.deconv().do_deconv())getDeconvImage(state.deconv(),all_ptr->add_response());
	if(state.has_map())getFeatureMap(state.map(),all_ptr->add_response());
	if(state.has_weight()&&state.weight().index()>=0&&(deconv_able_layer_types[state.weight().index()]=="Convolution"||deconv_able_layer_types[state.weight().index()]=="Deconvolution"))
	{
		getWeightForTest(state.weight(),all_ptr->add_response());
	}
	string out;
	if(all_ptr->response_size()<1)return;
	all_ptr->SerializeToString(&out);
	boost::shared_ptr<VResponse> rep(new VResponse);
	rep->set_type(VResponse_Type_ALL);
	rep->set_data(out);
	notifyListeners(rep);
}

void Detector::getWeightForTest(const VReqWeight &req,VResponse* rep_ptr)
{
	if(rep_ptr==NULL)
	{
		cerr<<"getWeightForTest VResponse point is null"<<endl;
		return ;
	}
	shared_ptr<Blob<float> > blob = deconv_able_layers[req.index()]->blobs()[0]; //todo
	// string layer_name = deconv_able_layer_names[req.index()];
	// int lr_able_layers_size = lr_able_layer_names.size();
	// int index = -1;
	// const vector<Blob<float>* > &lr_param = m_caffe_net->learnable_params();
	// for(int i=0;i<lr_able_layers_size;++i)
	// {
	// 	index++;
	// 	if(layer_name==lr_able_layer_names[i])
	// 	{
	// 		break;
	// 	}
	// }
	//cout<<index<<endl;
	//Blob<float>* blob = lr_param[index*2];
	//cout<<blob->num()<<" "<<blob->channels()<<" "<<blob->width()<<" "<<blob->height();

	int num = blob->num();
	int w = blob->width();
	int h = blob->height();
	int c = blob->channels();
	const float* fl_weight;
	if(isTrain&&req.diff())
	{
		fl_weight = blob->cpu_diff();
	}else{
		fl_weight = blob->cpu_data();
	}
	int count = blob->count();
	string weights(count,1);
	int size ;
	if(c==3)
	{
		size = w*h*c;
		for(int i=0;i<num;++i)
		{
			normTo255((unsigned char*)const_cast<char*>(weights.data()+i*size),fl_weight+i*size,size);
		}
	}else{
		size = w*h;
		for(int i=0;i<num*c;++i)
		{
			normTo255((unsigned char*)const_cast<char*>(weights.data()+i*size),fl_weight+i*size,size);
		}
	}
	// cout<<"count::"<<count<<endl;
	// for(int i=0;i<count;++i)
	// {
	// 	cout<<"|"<<fl_weight[i]<<" "<<(int)weights[i]<<"|";
	// }
	//cout<<endl;
	VFeatureMap* feature = new VFeatureMap();
	feature->set_width(w);
	feature->set_height(h);
	feature->set_num(num);
	feature->set_channel(c);

	feature->set_data(weights);
	rep_ptr->set_type(VResponse_Type_WEIGHT); 
	string fes_str;
	feature->SerializeToString(&fes_str);
	rep_ptr->set_data(fes_str);
	//delete []weights;
	delete feature;
}


void Detector::notifyListeners(boost::shared_ptr<VResponse>msg)
{
	if(msg==NULL)return;
	for(int i=0;i<listeners.size();++i)
	{
		Callback *cb = listeners[i];
		cb->haveMesssage(msg);
	}
}

void Detector::addListeners(Callback* cb)
{
	if(cb==NULL)return;
	listeners.push_back(cb);
}

void Detector::rmListeners(Callback*cb)
{
	if(cb==NULL)return;
	for(vector<Callback*>::iterator iter=listeners.begin(); iter!=listeners.end(); )  
	{  
		if( *iter == cb)  
			iter = listeners.erase(iter);  
		else  
			iter++;  
	} 
}

void Detector::setState(VState& state)
{
	this->state = state;
	string msg ;
	TextFormat::PrintToString(this->state,&msg);
	cout<<"Set State:"<<endl<<msg<<endl;
}


}