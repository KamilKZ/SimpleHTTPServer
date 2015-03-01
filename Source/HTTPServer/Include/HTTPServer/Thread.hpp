#pragma once

#include <thread>
#include <atomic>
#include "Common.hpp"

class API_EXPORT Thread // Dummy, this has to be exported for Start()...
{
public:
	Thread();
	virtual ~Thread();

	bool Start();
	void Stop(bool immediate = false);
	bool Join();
	bool Detach();
	std::thread::id GetThreadID();

	bool IsRunning(void) const { return mRunning; }

	virtual void* Run() = 0;
	
protected:
	std::thread mThread;
	std::atomic<bool> mRunning;
	std::atomic<bool> mDetached;
};