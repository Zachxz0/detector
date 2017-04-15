#ifndef _I_DETECTOR_H_
#define _I_DETECTOR_H_

namespace boost{
template<typename point>
class shared_ptr;
}

namespace zoson
{

class IDetector
{
public:
	int init();
	void deInit();
    virtual boost::shared_ptr<DetectOutput> doDetect(Mat input);//for detect
	virtual void doTrain();//for train
    virtual void getFeatureMap(const VReqFeature&,VResponse*)=0;
	virtual void getDeconvAbleLayers(VResponse*)=0;
    virtual void getOriginalImage(int i,VResponse*)=0;
    virtual void getDeconvImage(const VReqDeconv &,VResponse*)=0;
	virtual void getWeightForTest(int,VResponse*)=0;
	virtual void setState(VState&)=0;
};


#endif