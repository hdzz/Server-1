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

	//消息处理，返回false没有处理该消息。消息的删除需要外部处理
	bool Handle(Message* msg);

private:
	void Clear();
private:
	HandleMap	m_mapHandles;
};

