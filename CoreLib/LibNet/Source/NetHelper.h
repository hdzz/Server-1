#pragma once
#include "SharePtrDef.h"

class NetHelper
{
public:

	static TCPSession* GetSession(const TCPConnectionPtr& pConnect);

	static void ClearConnect(TCPConnectionPtr& pConnect);
};
