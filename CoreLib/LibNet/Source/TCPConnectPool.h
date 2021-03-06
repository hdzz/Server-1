#pragma once
#include <functional>
#include "SharePtrDef.h"
#include "IPacketHead.h"

class TCPConnectPool
{
public:
	TCPConnectPool(IOContext* pContext, IPacketHead* pHead);
	~TCPConnectPool();

public:

	std::function<void(TCPConnectionPtr&, std::string*)> OnRecivePacket;

	std::function<void(TCPConnectionPtr& , std::error_code )> OnConnectiontError;

	virtual void Stop() = 0;

	size_t CalDateLen(const char* pHead, size_t len);

	void OnRecive(TCPConnectionPtr pConnect, std::string* pPacket);

	void OnError(TCPConnectionPtr pConnect, std::error_code ec);



protected:
	IOContext*  m_pContext;
	IPacketHead* m_pHead;
	std::mutex m_Mutex;
};

