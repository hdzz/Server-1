#include "UniqueService.h"


UniqueService::UniqueService(BaseApp& app, uint16_t iTypeId)
	: BaseService(app, iTypeId)
{
}

UniqueService::~UniqueService()
{
}

void UniqueService::HandleMsg()
{
	m_ReciveMsg.Swap(m_SwapQueue);
	while (!m_SwapQueue.empty())
	{
		Message* pMsg = m_SwapQueue.front();
		m_MsgHandle.Handle(pMsg);
		delete pMsg;
		m_SwapQueue.pop();
	}
}
