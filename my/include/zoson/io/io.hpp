#ifndef _IO_HPP_
#define _IO_HPP_

namespace zoson
{
template<typename T>
class Input
{
public:
	virtual void getIn(T in)=0;
};

template<typename T>
class Output
{
public:
	virtual void getOut(T out)=0;
};

template<typename T>
class IOput:public Input<T>,Output<T>
{
	virtual void getIn(T in)=0;
	virtual void getOut(T out)=0;
};

}

#endif