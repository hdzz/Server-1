#pragma once
#include <atomic>
#include <thread>
#include <mutex>
#include <functional>

class WorkThread
{
public:
	WorkThread();
	~WorkThread();

	//Æô¶¯
	bool Start(const std::function<void(void)>& loopFunc);

	//ÔÝÍ£
	void Pause();
	//»Ö¸´
	void Resume();

	void Stop();

private:
	std::atomic_bool			m_bRun;
	std::thread*				m_pThread = nullptr;
	std::mutex					m_Mutex;
};
