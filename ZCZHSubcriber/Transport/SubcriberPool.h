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
		std::map<int, std::shared_ptr<DataSubcriber>> m_subcriberInUse; ///<���õ�
		std::list<int> m_Handles;		    ///�����
		const int MAX_HANDLE_COUNT = 64;	///�������
		const int MAGIC_NUM = 0xCE41;	    ///�����ʶ����
	};
}