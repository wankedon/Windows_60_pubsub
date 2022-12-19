#include "pch.h"
#include "ZCZHSubcriber.h"
#include "SubcriberPool.h"
#include "DataSubcriber.h"
#include "Logger.h"

namespace ZCZHSubcriber
{
	using namespace std;

	SubcriberPool::SubcriberPool()
	{
		for (auto i = 0; i < MAX_HANDLE_COUNT; i++)
		{
			m_Handles.push_back(MAGIC_NUM + i);   //构造函数中构建MAX_HANDLE_COUNT个备用句柄
		}
	}

	SubcriberPool::~SubcriberPool()
	{
		
	}

	std::pair<int, ZCZH_Error> SubcriberPool::newSubcriber(const ConfigLoader::BusConfig& config, const ResultCallBacks& rcbs)
	{
		if (m_Handles.empty())
			return std::pair<int, ZCZH_Error>{-1, ZCZH_ERR_NO_HANDLE_RESOURCE};
		auto subcriber = make_shared<DataSubcriber>(config, rcbs);
		int handle = m_Handles.front();
		subcriber->setSubscriberId(handle);
		assert(m_subcriberInUse.find(handle) == m_subcriberInUse.end());
		m_subcriberInUse[handle] = subcriber;
		m_Handles.pop_front();
		LOG("Register {} CurrentSize {}", handle, m_subcriberInUse.size());
		return std::pair<int, ZCZH_Error>{handle, ZCZH_ERR_NONE};
	}

	ZCZH_Error SubcriberPool::closeSubcriber(int handle)
	{
		if (m_subcriberInUse.find(handle) == m_subcriberInUse.end())
			return ZCZH_ERR_INVALID_HANDLE;
		m_subcriberInUse[handle]->release();
		m_subcriberInUse.erase(handle); //排除subcriber，自动清理
		m_Handles.push_back(handle);    //归还句柄号
		LOG("Unregister {} CurrentSize {}", handle, m_subcriberInUse.size());
		return ZCZH_ERR_NONE;
	}
}