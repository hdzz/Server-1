#pragma once
#include <Sdp/SdpUnpacker.h>

#include "IMsgHandle.h"
#include "ErrorCode.h"
template<class THandle, class TRequest, class TRespon>
class ServiceRequestRespon : IMsgHandle
{
public:
	using HandleFunc = void(THandle::*)(const TRequest& requst, TRespon& respon);
	ServiceRequestRespon(BaseService& service, THandle* pHandler, HandleFunc func)
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
			ByteMessage realMsg = static_cast<ByteMessage>(msg);
			ClientMessgae<TRespon>* repson = new ClientMessgae<TRespon>(msg.MsgId(), msg.To(), msg.From(), msg.ResponId());
			TRequest request;
			//反序列化
			const void* data = realMsg->Data().data() + msg->HeadSize();
			uint32_t size = realMsg.Data().size() - msg->HeadSize();
			SdpUnpacker sdp(data, size);
			sdp.unpack(request);

			(m_pHandler->*m_HandleFunc)(request, repson->Value());
			m_service.SendMsg(repson);
		}
		else if(msg->Type() == eMessage_Service)
		{
			ClientMessgae<TRespon>* repson = new ClientMessgae<TRespon>(msg.MsgId(), msg.To(), msg.From(), msg.ResponId());
			ServiceMessage<TRequest>* request = static_cast<ServiceMessage<TRequest>*>(msg);
			(m_pHandler->*m_HandleFunc)(request->Value(), repson->Value());
			m_service.SendMsg(repson);
		}
		else
		{
			return false;
		}
		return true;
	}

	virtual std::string ToString(const Message* msg) override
	{
		std::string strOut;
		//TODO:格式化到字符串
		return strOut;
	}

private:
	BaseService& m_service;
	THandle*	 m_pHandler;
	HandleFunc   m_HandleFunc;
};