#pragma once
#include <atomic>
#include <thread>
#include <queue>
#include <condition_variable>

template<class T>
class IOThread
{
public:
	IOThread() =default;
	~IOThread()
	{
		if (m_pThread != nullptr)
		{
			delete m_pThread;
		}
		while (!m_WaiteQueue.empty())
		{
			T* value = m_WaiteQueue.front();
			delete value;
			m_WaiteQueue.pop();
		}
	}

	void Push(T* value)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_WaiteQueue.push(value);
		m_Condition.notify_one();
	}

	void Stop()
	{
		m_bRun.store(false);
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Condition.notify_one();
		}
		if (m_pThread->joinable())
		{
			m_pThread->join();
		}
		m_HandleQueue.swap(m_WaiteQueue);
		Handle();
	}

	bool Start(const std::function<void(T*)>& func)
	{
		if (m_pThread != nullptr)
			return false;
		m_Func = func;
		m_bRun.store(true);
		m_pThread = new std::thread([this]() 
		{
			while (m_bRun)
			{
				Swap();
				Handle();
			}
		});
		return true;
	}

protected:
	void Swap()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		if (m_WaiteQueue.empty())
		{
			m_Condition.wait(lock);
		}
		m_WaiteQueue.swap(m_HandleQueue);
	}

	void Handle()
	{
		while (!m_HandleQueue.empty())
		{
			T* value = m_HandleQueue.front();
			m_Func(value);
			m_HandleQueue.pop();
		}
	}


private:
	std::atomic_bool			m_bRun;
	std::thread*				m_pThread = nullptr;
	std::condition_variable		m_Condition;
	std::mutex					m_Mutex;
	std::function<void(T*)>		m_Func;
	std::queue<T*>				m_WaiteQueue;
	std::queue<T*>				m_HandleQueue;
};
