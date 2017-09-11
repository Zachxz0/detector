#include <base/object.hpp>
#include <chrono>
#include <thread>
namespace zoson
{

Object::Object()
{

}

Object::~Object()
{

}

void Object::notify()
{
	std::unique_lock<std::mutex> lck(m_mutex);
	m_cv.notify_one();
}

void Object::notifyAll()
{
	std::unique_lock<std::mutex> lck(m_mutex);
	m_cv.notify_all();
}

void Object::wait()
{
	std::unique_lock<std::mutex> lck(m_mutex);
	m_cv.wait(lck);
}

void Object::wait(long time)
{
	std::unique_lock<std::mutex> lck(m_mutex);
	std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

}