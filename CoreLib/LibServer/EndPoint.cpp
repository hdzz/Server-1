#include "EndPoint.h"
#include "Message.h"
#include "BaseApp.h"
#include "ID.h"
size_t ServerPacketHead::HeadLen()
{
	return 12;
}

size_t ServerPacketHead::GetDataLen(const char * pHead, size_t iHeadLen)
{
	uint32_t iLen = 0;
	for (auto i = 0 + 8; i < 4 + 8; ++i)
	{
		iLen |= (pHead[i] & 0xFF) << (i * 8);
	}
	return iLen;
}


EndPoint::EndPoint(BaseApp* pApp, uint16_t iPort)
	: m_Pool(1)
	, m_pApp(pApp)
	, m_Server(m_Pool.GetIOContext(), &m_Head, iPort)
	, m_Client(m_Pool.GetIOContext(),&m_Head)
{
	m_Server.OnRecivePacket = std::bind(&EndPoint::OnRecive, this, std::placeholders::_1, std::placeholders::_2);
	m_Server.OnConnectiontError = std::bind(&EndPoint::OnError, this, std::placeholders::_1, std::placeholders::_2);

	m_Client.OnRecivePacket = std::bind(&EndPoint::OnRecive, this, std::placeholders::_1, std::placeholders::_2);
	m_Client.OnConnectiontError = std::bind(&EndPoint::OnError, this, std::placeholders::_1, std::placeholders::_2);
	m_Client.OnConnected = std::bind(&EndPoint::OnConnected, this, std::placeholders::_1);
}

EndPoint::~EndPoint()
{
}

bool EndPoint::Connect(const std::string & strIP, uint16_t iPort, uint32_t iId)
{
	TCPSession* pSession = m_Sessions.GetEntry(iId);
	if (pSession == nullptr || pSession->IsConnection())
		return false;
	pSession->SetUserToken(iId);
	m_Client.Connect(strIP, iPort, pSession);
	return true;
}

void EndPoint::Run()
{
	m_Pool.Run();
}

void EndPoint::Stop()
{
	m_Pool.Stop();
}

bool EndPoint::SendMsg(Message * pMsg, uint32_t subId)
{
	TCPSession* pSession = m_Sessions.GetEntry(subId);
	if (pSession == nullptr)
		return false;
	pSession->SendPacket(pMsg->Serialize());
	return true;
}

void EndPoint::OnRecive(TCPConnectionPtr& pConn, std::string * pPacket)
{
	ByteMessage* pMsg = new ByteMessage(std::shared_ptr<std::string>(pPacket));
	auto typeID = ID::Type(pMsg->MsgId());
	if (typeID != 0)
	{
		m_pApp->OnNewMsg(pMsg);
	}
	else
	{
		//TODO:收到消息后如果是系统消息，此处应该特殊处理
	}
}

void EndPoint::OnError(TCPConnectionPtr& pConnect, std::error_code ec)
{
	TCPSession* pSession = NetHelper::GetSession(pConnect);
	NetHelper::ClearConnect(pConnect);
	pSession->SetConnection(nullptr);
	if (pSession->UserToken() < m_pApp->ID())
	{
		//主动发起连接
	}
}

void EndPoint::OnConnected(TCPSession * pSession)
{
}
