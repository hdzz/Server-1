#include "NetHelper.h"
#include "../Internal/TCPConnection.h"

TCPSession * NetHelper::GetSession(const TCPConnectionPtr& pConnect)
{
	return pConnect->Session();
}

void NetHelper::ClearConnect(TCPConnectionPtr & pConnect)
{
	pConnect->SetSession(nullptr);
}
