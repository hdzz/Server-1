#pragma once

#include "Message.h"
class IMsgHandle
{
public:
	IMsgHandle() {}
	virtual				~IMsgHandle() {}

	virtual bool		 Handle(const Message* msg) = 0;

	virtual std::string	 ToString(const Message* msg) = 0;

private:
	IMsgHandle(const IMsgHandle &other) = delete;
	IMsgHandle &operator=(const IMsgHandle &other) = delete;
};