#pragma once
#include "IMsgHandle.h"
#include "ErrorCode.h"
template<class THandle, class TRequest, class TRespon>
class ClientRequestRespon : IMsgHandle
{
public:
	using HandleFunc = ErrorCode(THandle::*)(uint64_t playerId, const TRequest& requst, TRespon& respon);
	ClientRequestRespon(BaseService& service, THandle* pHandler, HandleFunc func)
		: m_service(service)
		, m_pHandler(pHandler)
		, m_HandleFunc(func)
	{

	}

	// 通过 IMsgHandle 继承
	virtual bool Handle(const Message* msg) override
	{
		if (msg.Type() == eMessage_Byte)
		{
			ByteMessage* realMsg = static_cast<ByteMessage*>(msg);
			BufferReader reader(realMsg->Data().data() + msg->HeadSize(), realMsg->Data().size() - msg->HeadSize());
			size_t iDataPos = msg->HeadSize() + 8;
			uint64_t playerId = 0;
			reader.Read(playerId);
			TRequest request;
			//反序列化
			const void* data = realMsg->Data().data() + iDataPos;
			uint32_t size = realMsg.Data().size() - iDataPos;
			SdpUnpacker sdp(data, size);
			sdp.unpack(request);

			ClientMessgae<TRespon>* repson = new ClientMessgae<TRespon>(msg.MsgId(), msg.To(), msg.From(), msg.ResponId());
			ErrorCode er = (m_pHandler->*m_HandleFunc)(playerId, request, repson->Value());
			if (!er)
			{
				m_service.SendMsg(repson);
			}
		}
		return true;
	}

	virtual std::string ToString(const Message* msg) override
	{
		std::string strOut;
		if (msg.Type() == eMessage_Byte)
		{
			ByteMessage* realMsg = static_cast<ByteMessage*>(msg);
			BufferReader reader(realMsg->Data().data() + msg->HeadSize(), realMsg->Data().size() - msg->HeadSize());
			size_t iDataPos = msg->HeadSize() + 8;
			uint64_t playerId = 0;
			reader.Read(playerId);
			TRequest request;
			//反序列化
			const void* data = realMsg->Data().data() + iDataPos;
			uint32_t size = realMsg.Data().size() - iDataPos;
			SdpUnpacker sdp(data, size);
			sdp.unpack(request);

			//TODO：格式化到字符串
			
		}
		return strOut;
	}

private:
	BaseService& m_service;
	THandle*	 m_pHandler;
	HandleFunc   m_HandleFunc;
};
