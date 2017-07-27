#pragma once
#include <string>

class BufferReader
{
public:
	BufferReader(const char* pData, uint32_t len)
		:m_pData(pData)
		, m_iLen(len)
		, m_iCap(len)
	{
	}

	BufferReader(const std::string& strData)
		:m_pData(strData.data())
		, m_iLen(strData.size())
		, m_iCap(strData.size())
	{

	}

	uint32_t ReadSize()
	{
		return m_iCap - m_iLen;
	}

	BufferReader& Read(uint16_t & value)
	{
		value = 0;
		if (m_iLen >= 2)
		{
			value = m_pData[0] & 0xff | m_pData[1] >> 8;
			m_pData += 2;
			m_iLen -= 2;
		}
		return *this;
	}

	BufferReader& Read(int16_t & value)
	{
		value = 0;
		if (m_iLen >= 2)
		{
			value = m_pData[0] & 0xff | m_pData[1] >> 8;
			m_pData += 2;
			m_iLen -= 2;
		}
		return *this;
	}

	BufferReader& Read(uint32_t & value)
	{
		value = 0;
		if (m_iLen >= 4)
		{
			for (auto i = 0; i < 4; ++i)
			{
				value |= (m_pData[i] & 0xFF) << (i * 8);
			}
			m_pData += 4;
			m_iLen -= 4;
		}
		return *this;
	}

	BufferReader& Read(int32_t & value)
	{
		value = 0;
		if (m_iLen >= 4)
		{
			for (auto i = 0; i < 4; ++i)
			{
				value |= (m_pData[i] & 0xFF) << (i * 8);
			}
			m_pData += 4;
			m_iLen -= 4;
		}
		return *this;
	}

	BufferReader& Read(uint64_t & value)
	{
		value = 0;
		if (m_iLen >= 8)
		{
			for (auto i = 0; i < 8; ++i)
			{
				value |= ((uint64_t)(m_pData[i] & 0xFF)) << (i * 8);
			}
			m_pData += 8;
			m_iLen -= 8;
		}
		return *this;
	}

	BufferReader& Read(int64_t & value)
	{
		value = 0;
		if (m_iLen >= 8)
		{
			for (auto i = 0; i < 8; ++i)
			{
				value |= ((uint64_t)(m_pData[i] & 0xFF)) << (i * 8);
			}
			m_pData += 8;
			m_iLen -= 8;
		}
		return *this;
	}


private:
	const char* m_pData;
	uint32_t m_iLen;
	uint32_t m_iCap;
};

class BuffWriter
{
public:
	BuffWriter(std::string& buffer, uint32_t pos = 0)
		:m_Buffer(buffer)
		,m_iWritePos(pos)
	{
	}

	BuffWriter& Write(uint16_t value)
	{
		CheckSize(2);
		for (auto i=0; i< 2; ++i)
		{
			m_Buffer[m_iWritePos++] = value >> (i * 8);
		}
		return *this;
	}

	BuffWriter& Write(int16_t value)
	{
		CheckSize(2);
		for (auto i = 0; i < 2; ++i)
		{
			m_Buffer[m_iWritePos++] = value >> (i * 8);
		}
		return *this;
	}

	BuffWriter& Write(uint32_t value)
	{
		CheckSize(4);
		for (auto i = 0; i < 4; ++i)
		{
			m_Buffer[m_iWritePos++] = value >> (i * 8);
		}
		return *this;
	}

	BuffWriter& Write(int32_t value)
	{
		CheckSize(4);
		for (auto i = 0; i < 4; ++i)
		{
			m_Buffer[m_iWritePos++] = value >> (i * 8);
		}
		return *this;
	}

	BuffWriter& Write(uint64_t value)
	{
		CheckSize(8);
		for (auto i = 0; i < 8; ++i)
		{
			m_Buffer[m_iWritePos++] = (char)(value >> (i * 8));
		}
		return *this;
	}

	BuffWriter& Write(int64_t value)
	{
		CheckSize(8);
		for (auto i = 0; i < 8; ++i)
		{
			m_Buffer[m_iWritePos++] = (char)(value >> (i * 8));
		}
		return *this;
	}
private:
	void CheckSize(int32_t iSize)
	{
		if (m_Buffer.size() < (size_t)m_iWritePos + (size_t)iSize)
		{
			m_Buffer.append((size_t)m_iWritePos + (size_t)iSize - m_Buffer.size(), char(0));
		}
	}
private:
	std::string& m_Buffer;
	uint32_t m_iWritePos;
};
