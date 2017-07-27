#include "WorkThread.h"

WorkThread::WorkThread()
{
}

WorkThread::~WorkThread()
{
	if (m_pThread != nullptr)
		delete m_pThread;
}

bool WorkThread::Start(const std::function<void(void)>& loopFunc)
{
	if (m_pThread != nullptr)
		return false;
	m_pThread = new std::thread([this, loopFunc]()
	{
		while (m_bRun)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			loopFunc();
		}
	});
	return true;
}

void WorkThread::Pause()
{
	m_Mutex.lock();
}

void WorkThread::Resume()
{
	m_Mutex.unlock();
}

void WorkThread::Stop()
{
	m_bRun.store(false);
	if (m_pThread != nullptr)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		if (m_pThread->joinable())
		{
			m_pThread->join();
		}
	}
}
