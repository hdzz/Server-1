#pragma once
#include <memory>
#include <mutex>

#include "SharePtrDef.h"
class TCPConnection;
using TCPConnectionPtr = std::shared_ptr<TCPConnection>;

class TCPSession
{
public:
	TCPSession();
	~TCPSession();

	TCPSession(const TCPSession&) = delete;
	TCPSession& operator=(const TCPSession&) = delete;

	void DisConnect();

	bool IsConnection();

	void SetConnection(const TCPConnectionPtr& pConn);

	void SendPacket(const NetPacket& packet);

	//����Ϣ�������ǰ�ˣ����ڶ������������֤��Ϣ��
	void PushPacketFront(const NetPacket& packet);

	void SendPackets(const std::list<NetPacket>& packets);

	void SetUserToken(uint64_t iToken) { m_iUserToken = iToken; }

	uint64_t UserToken()const { return m_iUserToken; }

private:
	TCPConnectionPtr m_pConnection;
	PacketQueue		 m_PacketQueue;
	std::mutex		 m_Mutex;
	uint64_t		 m_iUserToken;
};

