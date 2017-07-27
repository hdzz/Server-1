#pragma once

#include <mutex>

template <class EntryType>
class Directory
{
public:
	Directory()
	{
		for (uint32_t page = 0; page < MAX_PAGES; ++page)
		{
			m_Pages[page] = nullptr;
		}
	}

	~Directory()
	{
		for (uint32_t page = 0; page < MAX_PAGES; ++page)
		{
			if (m_Pages[page])
			{
				delete m_Pages[page];
			}
		}
	}

	inline EntryType* GetEntry(const uint32_t index)
	{
		const uint32_t page(index / ENTRIES_PER_PAGE);
		const uint32_t offset(index % ENTRIES_PER_PAGE);
		if (page >= MAX_PAGES || offset >= ENTRIES_PER_PAGE)
			return nullptr;
		if (m_Pages[page] == nullptr)
		{
			Allocate(index);
		}

		return &(m_Pages[page]->mEntries[offset]);
	}

protected:

	void Allocate(uint32_t index ) 
	{
		m_Mutex.lock();

		const uint32_t page(index / ENTRIES_PER_PAGE);
		for (uint32_t i=0; i<= page; ++i)
		{
			if (m_Pages[i] == nullptr)
			{
				m_Pages[i] = new Page;
			}
		}
		m_Mutex.unlock();
	}


private:

	static const uint32_t ENTRIES_PER_PAGE = 256;  ///< Number of entries in each allocated page (power of two!).
	static const uint32_t MAX_PAGES = 32;         ///< Maximum number of allocated pages.

	struct Page
	{
		EntryType mEntries[ENTRIES_PER_PAGE];       ///< Array of entries making up this page.
	};

	Directory(const Directory &other) = delete;
	Directory &operator=(const Directory &other) = delete;

	mutable std::mutex		m_Mutex;                           ///< Ensures thread-safe access to the instance data.
	uint32_t				m_iNextIndex;                            ///< Auto-incremented index to use for next registered entity.
	Page*					m_Pages[MAX_PAGES];                        ///< Pointers to allocated pages.
};
