#pragma once
#include "ConfigLoader.h"

namespace ZCZHSubcriber
{
	class DataSubcriber;
	class SubcriberPool
	{
	public:
		SubcriberPool();
		~SubcriberPool();
	public:
		std::pair<int, ZCZH_Error> newSubcriber(const ConfigLoader::BusConfig& config,const ResultCallBacks& rcbs);
		ZCZH_Error closeSubcriber(int handle);
	private:
		std::map<int, std::shared_ptr<DataSubcriber>> m_subcriberInUse; ///<在用的
		std::list<int> m_Handles;		    ///句柄池
		const int MAX_HANDLE_COUNT = 64;	///句柄数量
		const int MAGIC_NUM = 0xCE41;	    ///句柄标识生成
	};
}