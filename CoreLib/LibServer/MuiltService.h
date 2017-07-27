#pragma once
#include <vector>
#include "BaseService.h"
#include "SubService.h"

class MuiltService : public BaseService
{
public:
	MuiltService(BaseApp& app, uint16_t iTypeId);
	~MuiltService();

	

protected:
	
	virtual void HandleMsg() override;

	virtual void Run(uint64_t curTime) override;

protected:
	std::vector<SubServicePtr> m_vSubService;
};
