#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdint.h>
#include <algorithm>
#include <csignal>
#include <map>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "caffe/data_transformer.hpp"
#include "caffe/layers/zoson_data_layer.hpp"
#include "caffe/util/benchmark.hpp"
#include <iostream>
using namespace std;
namespace caffe {

template <typename Dtype>
ZosonDataLayer<Dtype>::ZosonDataLayer(const LayerParameter& param)
  : BaseDataLayer<Dtype>(param) {
    //transformed_param_(param.transformed_param());
}

template <typename Dtype>
ZosonDataLayer<Dtype>::~ZosonDataLayer() {
}

template<typename Dtype>
void ZosonDataLayer<Dtype>::LayerSetUp(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  BaseDataLayer<Dtype>::LayerSetUp(bottom, top);
  // Before starting the prefetch thread, we make cpu_data and gpu_data
  // calls so that the prefetch thread does not accidentally make simultaneous
  // cudaMalloc calls when the main thread is running. In some GPUs this
  // seems to cause failures if we do not so.
  cout<<"ZosonDataLayer<Dtype>::LayerSetUp start"<<endl;
  batch.data_.mutable_cpu_data();
  cout<<"ZosonDataLayer<Dtype>::LayerSetUp 0"<<endl;
  if(this->output_labels_){
    batch.label_.mutable_cpu_data();
  }
#ifndef CPU_ONLY
  if (Caffe::mode() == Caffe::GPU) {
    batch.data_.mutable_gpu_data();
    if(this->output_labels_)
      batch.label_.mutable_gpu_data();
  }    
#endif
  DLOG(INFO) << "Initializing prefetch";
  this->data_transformer_->InitRand();
}

template<typename Dtype>
void ZosonDataLayer<Dtype>::DataLayerSetUp(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  const int batch_size = this->layer_param_.data_param().batch_size();
  // Read an image, and use it to initialize the top blob.
  cv::Mat cv_img;
  cv_img = cv::imread("../my/data/test/cat.jpg");
  CHECK(cv_img.data) << "Could not load image!";
  // Use data_transformer to infer the expected blob shape from a cv_image.
  top_shape_ = this->data_transformer_->InferBlobShape(cv_img);
  this->transformed_data_.Reshape(top_shape_);
  top_shape_[0] = batch_size;
  top[0]->Reshape(top_shape_);

  //batch
  batch.data_.Reshape(top_shape_);

  LOG(INFO) << "output data size: " << top[0]->num() << ","
      << top[0]->channels() << "," << top[0]->height() << ","
      << top[0]->width();
  // label
  if (this->output_labels_) {
    vector<int> label_shape(1, batch_size);
    top[1]->Reshape(label_shape);
    //batch
    batch.label_.Reshape(label_shape);
  }
}

template<typename Dtype>
void ZosonDataLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top)
{
  // Reshape to loaded data.
  top[0]->ReshapeLike(batch.data_);
  // Copy the data
  caffe_copy(batch.data_.count(), batch.data_.cpu_data(),
             top[0]->mutable_cpu_data());
  DLOG(INFO) << "Prefetch copied";
  if (this->output_labels_) {
    // Reshape to loaded labels.
    top[1]->ReshapeLike(batch.label_);
    // Copy the labels.
    caffe_copy(batch.label_.count(), batch.label_.cpu_data(),
        top[1]->mutable_cpu_data());
  }
}

template<typename Dtype>
void ZosonDataLayer<Dtype>::load_data(cv::Mat mat_img)
{
  CHECK(mat_img.data);
  CHECK(batch.data_.count());
  CHECK(this->transformed_data_.count());
  top_shape_[0] = 1;
  this->transformed_data_.Reshape(top_shape_);
  batch.data_.Reshape(top_shape_);

  Dtype* top_data = batch.data_.mutable_cpu_data();
  Dtype* top_label = NULL;
  if(this->output_labels_)
  {
    top_label = batch.label_.mutable_cpu_data();
  }
  int offset = batch.data_.offset(0);
  this->transformed_data_.set_cpu_data(top_data+offset);
  this->data_transformer_->Transform(mat_img,&(this->transformed_data_));
  if(this->output_labels_)
  {
    top_label[0] = 0;
  }
}


#ifdef CPU_ONLY
STUB_GPU_FORWARD(ZosonDataLayer,Forward);
#endif

INSTANTIATE_CLASS(ZosonDataLayer);
REGISTER_LAYER_CLASS(ZosonData);

}  // namespace caffe
#endif  // USE_OPENCV