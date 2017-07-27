#include "BaseService.h"



BaseService::BaseService(BaseApp& app, uint16_t iTypeId)
	: m_App(app)
	, m_iTypeId(iTypeId)
{
}


BaseService::~BaseService()
{
}

void BaseService::Start()
{
	m_WorkThread.Start([this]() 
	{
		HandleMsg();
	});
}

void BaseService::Pause()
{
	m_WorkThread.Pause();
}

void BaseService::Resume()
{
	m_WorkThread.Resume();
}

void BaseService::Stop()
{
	m_WorkThread.Stop();
	OnStop();
}

void BaseService::SendMsg(Message * msg)
{
	m_App.SendMsg(msg);
}

void BaseService::OnNewMsg(Message * pMsg)
{
	m_ReciveMsg.Push(pMsg);
}
