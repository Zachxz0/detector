#include <io/io.hpp>
#include <proto/zoson.pb.h>
namespace zoson
{

class RtmpInput
{
public:
	RtmpInput();
	~RtmpInput();
	void setInput(Input<Mat>* in);
protected:

private:
	Input<Mat> *input;
};

}