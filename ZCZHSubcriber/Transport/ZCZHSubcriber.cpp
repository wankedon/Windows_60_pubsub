#include "pch.h"
#include "ZCZHSubcriber.h"
#include "DataSubcriber.h"
#include "SubcriberPool.h"
#include "ConfigLoader.h"
#include "StringConv.h"
#include "Miscellaneous.h"
#include "Logger.h"

namespace ZCZHSubcriber
{
	std::unique_ptr<ConfigLoader::BusConfig> config;
	std::unique_ptr<SubcriberPool> subcriberPool;
	ZCZHSUBCRIBER_API ZCZH_Error InitLib()
	{
		time_t ttnow = time(NULL);
		tm* tmnow = localtime(&ttnow);
#ifdef PLATFROM_WINDOWS
		std::wstring path = LocateModulePath();
		auto logpath = StrConvert::UTF8ToGbk(StrConvert::wstringToUTF8(path));
		Logger::PATH = fmt::format("{}\\subcriberlog\\subcriberlog_{:04}-{:02}-{:02}.log", logpath, tmnow->tm_year + 1900, tmnow->tm_mon + 1, tmnow->tm_mday);
#else
		char* cpath = getcwd(NULL, 0);
		std::string strPath = cpath;
		std::wstring path = StrConvert::UTF8Towstring(strPath);
		Logger::PATH = fmt::format("{}/subcriberlog/subcriberlog_{:04}-{:02}-{:02}.log", strPath, tmnow->tm_year + 1900, tmnow->tm_mon + 1, tmnow->tm_mday);
#endif
		ConfigLoader configLoader(path + L"/ConfigFile.xml");
		auto configPtr = configLoader.load();
		if (configPtr != nullptr)
		{
			config.swap(configPtr);
		}
		else
		{
			return ZCZH_ERR_READ_CONFIG;
		}
		if (subcriberPool == nullptr)
		{
			subcriberPool = std::make_unique<SubcriberPool>();
		}
		LOG("InitLib");
		return ZCZH_ERR_NONE;
	}

	ZCZHSUBCRIBER_API ZCZH_Error RegisterSubscriber(int* subcriberId, const ResultCallBacks& rcbs)
	{
		if (subcriberPool == nullptr)
			return ZCZH_ERR_INIT;
		if (config == nullptr)
			return ZCZH_ERR_READ_CONFIG;
		auto pair = subcriberPool->newSubcriber(*config,rcbs);
		*subcriberId = pair.first;
		return pair.second;
	}

	ZCZHSUBCRIBER_API ZCZH_Error UnregisterSubscriber(int subcriberId)
	{
		if (subcriberPool == nullptr)
			return ZCZH_ERR_INIT;
		if (subcriberId == -1)
			return ZCZH_ERR_INVALID_HANDLE;
		return subcriberPool->closeSubcriber(subcriberId);
	}

	ZCZHSUBCRIBER_API ZCZH_Error DeInitLib()
	{
		if (subcriberPool == nullptr)
			return ZCZH_ERR_INIT;
		subcriberPool.reset();
		if (config == nullptr)
			return ZCZH_ERR_READ_CONFIG;
		config.reset();
		zsys_shutdown();
		LOG("DeInitLib");
		return ZCZH_ERR_NONE;
	}
}