#pragma once
#include <map>
#include <memory>
#include <LibNet.h>
#include "IOThread.h"
#include "Message.h"

class BaseService;
using ServicePtr = std::shared_ptr<BaseService>;
class EndPoint;
class BaseApp
{
	using ServiceMap = std::map<uint32_t, ServicePtr>;
public:
	BaseApp();
	virtual ~BaseApp();

	void RegistService(ServicePtr pService);

	void OnNewMsg(Message* pMsg);

	void SendMsg(Message* pMsg);

	bool Init();

	void Run();

	void Stop();

	uint16_t ID()const { return m_iID; }

protected:
	void DispatchMsg(Message* pMsg);

protected:
	uint16_t	m_iID;
	ServiceMap	m_Service;
	EndPoint*	m_pEndPoint = nullptr;
	ThreadQueue<Message*> m_ReciveMsg;
	IOThread<Message>	m_SendThread;
};

