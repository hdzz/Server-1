#pragma once
#include <sstream>

class ID
{
private:
	union Address
	{
	public:
		explicit Address(uint16_t id):Id(id)
		{
		}
		explicit Address(int32_t type, int32_t subid)
		{
			Detail.type = type;
			Detail.subId = subid;
		}
		uint16_t Id;
		struct 
		{
			uint16_t subId : 10;
			uint16_t type : 6;
		}Detail;
	};

public:
	static int Type(uint16_t id)
	{
		Address ad(id);
		return ad.Detail.type;
	}

	static int SubID(uint16_t id)
	{
		Address ad(id);
		return ad.Detail.subId;
	}
	static std::string ToString(uint16_t id)
	{
		Address ad(id);
		ad.Detail.type;
		std::stringstream st;
		st << ad.Detail.type << "-" << ad.Detail.subId;
		return st.str();
	}

	static uint16_t FromString(std::string& )
	{
		
	}

	static uint16_t Make(int32_t type, int32_t subId)
	{
		Address ad(type, subId);
		return ad.Id;
	}
};