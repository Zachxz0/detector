#ifndef CAFFE_ZOSON_DATA_LAYER_HPP_
#define CAFFE_ZOSON_DATA_LAYER_HPP_

#ifdef USE_OPENCV
#if OPENCV_VERSION == 3
#include <opencv2/videoio.hpp>
#else
#include <opencv2/opencv.hpp>
#endif  // OPENCV_VERSION == 3

#include <string>
#include <vector>

#include "caffe/blob.hpp"
#include "caffe/data_transformer.hpp"
#include "caffe/internal_thread.hpp"
#include "caffe/layer.hpp"
#include "caffe/layers/base_data_layer.hpp"
#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"

namespace caffe {

/**
 * @brief Provides data to the Net from the way defined by zoson.
 *
 * TODO(zoson): thorough documentation for Forward and proto params.
 */
template <typename Dtype>
class ZosonDataLayer : public BaseDataLayer<Dtype> {
 public:
  explicit ZosonDataLayer(const LayerParameter& param);
  virtual ~ZosonDataLayer();
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void DataLayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual inline bool ShareInParallel() const { return false; }
  virtual inline const char* type() const { return "ZosonData"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void load_data(cv::Mat data);
 protected:
  vector<int> top_shape_;
  Batch<Dtype> batch;
  Blob<Dtype> transformed_data_;
};

}  // namespace caffe
#endif  // USE_OPENCV

#endif  // CAFFE_ZOSON_DATA_LAYER_HPP_