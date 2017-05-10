#ifndef _D_CONTEXT_HPP_
#define _D_CONTEXT_HPP_

#include <proto/zoson.pb.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <string>
#include <iostream>
using namespace std;
namespace zoson
{
class Detector;
class Communicator;

class DContext
{
public:
	DContext(const DContextParameter& param);
	DContext(const string& param_path);
	virtual ~DContext();
	boost::shared_ptr<Detector> getDetector(string name);
	boost::shared_ptr<Detector> getDetector(int index);
	boost::shared_ptr<Communicator> getCommu();
protected:
	DContext(const DContext&){};
	void initByParam(const DContextParameter&param);

private:
	std::vector<boost::shared_ptr<Detector> > m_detectors;
	boost::shared_ptr<Communicator> m_commu;
	DContextParameter m_param;
};

}

#endif
