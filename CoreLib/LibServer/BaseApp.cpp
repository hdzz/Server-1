#include "BaseApp.h"
#include "BaseService.h"
#include "ID.h"
#include "EndPoint.h"
#include <iostream>
BaseApp::BaseApp()
{
}


BaseApp::~BaseApp()
{
	if (m_pEndPoint != nullptr)
		delete m_pEndPoint;
}

void BaseApp::RegistService(ServicePtr pService)
{
	if (m_Service.find(pService->TypeId()) == m_Service.end())
	{
		m_Service.insert(std::make_pair(pService->TypeId(), pService));
	}
}

void BaseApp::OnNewMsg(Message * pMsg)
{
	auto typeID = ID::Type(pMsg->MsgId());
	if (typeID != 0)
	{
		auto it = m_Service.find(typeID);
		if (it != m_Service.end())
		{
			it->second->OnNewMsg(pMsg);
		}
	}
	else
	{
		m_ReciveMsg.Push(pMsg);
	}
}

void BaseApp::SendMsg(Message * pMsg)
{
	m_SendThread.Push(pMsg);
}

bool BaseApp::Init()
{
	std::cout << "App Init" << std::endl;
	for (auto it : m_Service)
	{
		bool bResult = it.second->Init();
		std::cout << " Service : " << it.second->Name() << " Init : " << (bResult ? "Success" : "Faile") << std::endl;
		if (!bResult)
			return false;
	}
	return true;
}

void BaseApp::DispatchMsg(Message* pMsg)
{
	auto subId = ID::SubID(pMsg->MsgId());
	if (subId != m_iID)
	{
		m_pEndPoint->SendMsg(pMsg, subId);
	}
	else
	{
		OnNewMsg(pMsg);
	}
}

void BaseApp::Run()
{
	m_SendThread.Start(std::bind(&BaseApp::DispatchMsg, this, std::placeholders::_1));
	for (auto it : m_Service)
	{
		it.second->Start();
	}
}

void BaseApp::Stop()
{
	for (auto it : m_Service)
	{
		it.second->Stop();
	}

	m_SendThread.Stop();
}
