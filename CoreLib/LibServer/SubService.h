#pragma once
#include <memory>

class MuiltService;
class SubService
{
public:
	SubService();
	virtual ~SubService();
	
	uint32_t ID() { return m_iID; }

protected:
	MuiltService*	m_pService;
	uint32_t m_iID;
	friend class MuiltService;
};

using SubServicePtr = std::shared_ptr<SubService>;
