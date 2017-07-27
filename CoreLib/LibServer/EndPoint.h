#pragma once
#include <LibNet.h>
#include "Directory.h"

class ServerPacketHead : public IPacketHead
{
public:
	// Í¨¹ý IPacketHead ¼Ì³Ð
	virtual size_t HeadLen() override;

	virtual size_t GetDataLen(const char * pHead, size_t iHeadLen) override;

};

class BaseApp;
class Message;

class EndPoint
{
public:
	EndPoint(BaseApp* pApp, uint16_t iPort);

	~EndPoint();

	bool Connect(const std::string& strIP, uint16_t iPort, uint32_t iId);

	void Run();

	void Stop();

	bool SendMsg(Message* pMsg, uint32_t subId);

protected:
	void OnRecive(TCPConnectionPtr& pConn, std::string* pPacket);

	void OnError(TCPConnectionPtr& pConnect, std::error_code ec);

	void OnConnected(TCPSession* pSession);

private:
	BaseApp*	m_pApp;
	ServerPacketHead m_Head;
	TCPServer	m_Server;
	TCPClient	m_Client;
	IOContextPool m_Pool;
	Directory<TCPSession> m_Sessions;
};
