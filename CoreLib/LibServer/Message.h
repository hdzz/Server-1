#pragma once
#include <vector>
#include <memory>
#include <sdp/SdpPacker.h>
#include "BufferHelper.h"

enum MessageType
{
	eMessage_Service,
	eMessage_Client,
	eMessage_Byte,
};
class Message
{
public:
	Message(uint16_t msgid, uint16_t from, uint16_t to, MessageType type)
		: m_MsgId(msgid)
		, m_FromId(from)
		, m_DestId(to)
		, m_Type(type)
	{
	}

	Message(MessageType type)
		:m_Type(type)
	{

	}

	uint16_t MsgId()const  { return m_MsgId; }

	uint16_t From()const { return m_FromId; }

	uint16_t To()const { return m_DestId; }

	uint16_t ResponId()const { return m_ResponId; }

	MessageType Type()const { return m_Type; }

	virtual std::shared_ptr<std::string> Serialize()const = 0;

	uint32_t HeadSize()const { return 12; }

protected:
	uint16_t	m_MsgId = 0;
	uint16_t	m_ResponId = 0;
	uint16_t	m_FromId = 0;
	uint16_t	m_DestId = 0;
	MessageType m_Type;
};

template<class TMessage>
class ServiceMessage : public Message
{
public: 
	ServiceMessage(uint16_t msgid, uint16_t from, uint16_t to, uint16_t responId)
		: Message(msgid, from, to, eMessage_Client)
	{
	}

	ServiceMessage(uint16_t msgid, uint16_t from, uint16_t to, uint16_t responId, TMessage& msg )
		: Message(msgid, from, to, eMessage_Client)
		, m_Value(msg)
	{
	}

	TMessage& Value() { return m_Value; }

	
	virtual std::shared_ptr<std::string> Serialize()const override
	{
		std::shared_ptr<std::string> pMsg = std::make_shared<std::string>();
		std::string& strBuffer = *pMsg;
		strBuffer.append(HeadSize(), (char)0);
		BuffWriter write(strBuffer);
		write.Write(m_MsgId).Write(m_ResponId).Write(m_FromId).Write(m_DestId).Write((uint32_t)(strBuffer.size() - HeadSize()));
		//序列化
		SdpPacker sdp(strBuffer);
		sdp.pack(m_Value);
		strBuffer.swap(sdp.getData());
		return pMsg;
	}

protected:
	TMessage m_Value;
};

template<class TMessage>
class ClientMessgae : public Message
{
public:
	ClientMessgae(uint16_t msgid, uint16_t from, uint16_t to, uint16_t responId)
		: Message(msgid, from, to, eMessage_Service)
	{
	}

	ClientMessgae(uint16_t msgid, uint16_t from, uint16_t to, uint16_t responId, TMessage& msg)
		: Message(msgid, from, to, eMessage_Service)
		, m_Value(msg)
	{
	}
	TMessage& Value() { return m_Value; }

	std::vector<uint64_t>& PlayerIds() { return m_PlayerIds; }

	virtual std::shared_ptr<std::string> Serialize()const  override
	{
		std::shared_ptr<std::string> pMsg = std::make_shared<std::string>();
		std::string& strBuffer = *pMsg;
		uint32_t idLen = m_PlayerIds.size() * 8 + 2;
		strBuffer.append(HeadSize() + idLen, (char)0);
		BuffWriter write(strBuffer);
		write.Write(m_MsgId).Write(m_ResponId).Write(m_FromId).Write(m_DestId).Write((uint32_t)(strBuffer.size() - HeadSize()));
		write.Write((uint16_t)(m_PlayerIds.size() * 8));
		for (uint64_t id : m_PlayerIds)
		{
			write.Write(id);
		}

		//序列化
		SdpPacker sdp(strBuffer);
		sdp.pack(m_Value);
		strBuffer.swap(sdp.getData());

		return pMsg;
	}

protected:
	TMessage m_Value;
	std::vector<uint64_t> m_PlayerIds;
};


class ByteMessage : public Message
{
public:
	ByteMessage(const std::shared_ptr<std::string>& strData)
		:Message(eMessage_Byte)
		,m_Data(strData)
	{
		BufferReader reader(*m_Data);
		reader.Read(m_MsgId).Read(m_ResponId).Read(m_FromId).Read(m_DestId);
	}

	const std::string Data()const { return *m_Data; }

	virtual std::shared_ptr<std::string> Serialize()const override
	{
		return m_Data;
	}

private:
	std::shared_ptr<std::string> m_Data;
};
