#pragma once
#include "BaseService.h"
#include "HandlerCollection.h"

class UniqueService : public BaseService
{
public:
	UniqueService(BaseApp& app, uint16_t iTypeId);
	~UniqueService();

	// ͨ�� BaseService �̳�
	virtual void HandleMsg() override;

protected:
	HandlerCollection m_MsgHandle;
	std::queue<Message*> m_SwapQueue;
};
