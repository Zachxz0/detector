#include "app/dcontext.hpp"
#include "detector/detector.hpp"
#include "commu/commu.hpp"
#include <utils/common.hpp>
#include <string>
using namespace std;
using namespace boost;
namespace zoson
{

DContext::DContext(const DContextParameter& param):m_param(param)
{
	initByParam(param);
}

DContext::DContext(const string&param_file)
{
	DContextParameter param;
	read_proto_txt(&param,param_file);
	this->m_param = param;
	initByParam((const DContextParameter)param);
}

DContext::~DContext()
{
}

void DContext::initByParam(const DContextParameter& param)
{
	if(param.model_size()>0)
	{
		int count = param.model_size();
		for(int c=0;c<count;++c)
		{
			const DModel& dmodel = param.model(c);
			Detector* detector;
			if(dmodel.has_path())detector = new Detector(dmodel.path());
			if(!dmodel.has_path()&&dmodel.has_model())detector = new Detector(dmodel.model());
			this->m_detectors.push_back(shared_ptr<Detector>(detector));
		}
	}
	cout<<"init ing"<<endl;
	if(param.has_commu())
	{
		Communicator* commu = new Communicator(param.commu());
		this->m_commu.reset(commu);
		
	}
}

shared_ptr<Detector> DContext::getDetector(int index)
{
	shared_ptr<Detector> ptr;
	if(index>m_detectors.size()+1)return ptr;
	return m_detectors[index];
}

shared_ptr<Detector> DContext::getDetector(string name)
{
	shared_ptr<Detector> ptr;
	int size = m_detectors.size();
	for(int i=0;i<size;++i)
	{
		if(m_detectors[i]->getName()==name)
		{
			return m_detectors[i];
		}
	}
	return ptr;
}

}