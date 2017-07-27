#pragma once
#include <map>
#include "IMsgHandle.h"
class HandlerCollection
{
	using HandleMap = std::map<uint16_t, IMsgHandle*>;
public:
	HandlerCollection();
	~HandlerCollection();

	bool AddHandle(uint16_t msgId, IMsgHandle* msgHandler);

	//��Ϣ��������falseû�д������Ϣ����Ϣ��ɾ����Ҫ�ⲿ����
	bool Handle(Message* msg);

private:
	void Clear();
private:
	HandleMap	m_mapHandles;
};

