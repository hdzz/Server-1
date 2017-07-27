#pragma once
#include <LibNet.h>
#include "Message.h"
#include "BaseApp.h"
#include "WorkThread.h"

class BaseService
{
public:
	BaseService(BaseApp& app, uint16_t iTypeId);
	virtual ~BaseService();

	virtual bool Init() = 0;

	void Start();

	void Pause();

	void Resume();

	void Stop();

	void SendMsg(Message* msg);

	void OnNewMsg(Message* pMsg);

	uint16_t TypeId()const { return m_iTypeId; }

	const std::string& Name()const { return m_strName; }

protected:
	virtual void Run(uint64_t curTime) = 0;

	virtual void HandleMsg() = 0;

	virtual void OnStop() = 0;

protected:
	std::string				m_strName;
	uint16_t m_iTypeId;
	BaseApp& m_App;
	ThreadQueue<Message*>	m_ReciveMsg;
	WorkThread				m_WorkThread;
};

using ServicePtr = std::shared_ptr<BaseService>;

