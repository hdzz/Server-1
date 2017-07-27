#include "HandlerCollection.h"

class SafeMsg
{
public:
	SafeMsg(Message* pMsg)
		: m_pMsg(pMsg)
	{

	}
	~SafeMsg()
	{
		delete m_pMsg;
	}

private:
	Message* m_pMsg;
};


HandlerCollection::HandlerCollection()
{
}


HandlerCollection::~HandlerCollection()
{
	Clear();
}

bool HandlerCollection::AddHandle(uint16_t msgId, IMsgHandle * msgHandler)
{
	HandleMap::iterator it = m_mapHandles.find(msgId);
	if (it != m_mapHandles.end())
	{
		return false;
	}
	m_mapHandles.insert(std::make_pair(msgId, msgHandler));
	return true;
}


bool HandlerCollection::Handle(Message * msg)
{
	//SafeMsg safe(msg);
	HandleMap::iterator it = m_mapHandles.find(msg->MsgId());
	if (it != m_mapHandles.end())
	{
		it->second->Handle(msg);
		return true;
	}
	return false;
}

void HandlerCollection::Clear()
{
	for (auto& it : m_mapHandles)
	{
		delete it.second;
	}
	m_mapHandles.clear();
}
