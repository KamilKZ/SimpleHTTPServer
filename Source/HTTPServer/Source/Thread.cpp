#include <HTTPServer/Thread.hpp>


// http://vichargrave.com/java-style-thread-class-in-c/
// https://github.com/vichargrave/threads/blob/master/thread.cpp

Thread::Thread() : mRunning(false), mDetached(false) {};

Thread::~Thread()
{
	if (mRunning)
	{
		mRunning = false;
		if (!mDetached)
			Detach();
	}
}

bool Thread::Start()
{
	mRunning = true;
	mThread = std::thread(&Thread::Run, this);
	
	return true;
}

void Thread::Stop(bool immediate)
{
	mRunning = false;

	if (immediate)
	{
		Detach();
	}
	else
	{
		Join();
	}
}

bool Thread::Join()
{
	if (mRunning)
	{
		mThread.join();
		mDetached = false;
		return true;
	}
	return false;
}

bool Thread::Detach()
{
	if (mRunning && !mDetached)
	{
		mThread.detach();
		mDetached = true;
		return true;
	}
	return false;
}

std::thread::id Thread::GetThreadID()
{
	return mThread.get_id();
}