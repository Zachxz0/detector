#ifndef _I_PACKAGE_FACTORY_HPP_
#define _I_PACKAGE_FACTORY_HPP_

namespace linker
{

class IPackageFactory
{
public:
	virtual IPackage* newMemPackage()=0;
};

}


#endif 