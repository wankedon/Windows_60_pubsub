#include "pch.h"
#include "DataReceiver.h"
#include "ConfigLoader.h"
#include "Whiteboard.h"
#include "StringConv.h"
#include "Miscellaneous.h"
#include "Logger.h"
namespace DCTSDataTransport
{
	using namespace std;

	DataReceiver::DataReceiver(const InnerReceiveResultCallBacks& irrcbs)
		:m_irrcbs(irrcbs)
	{
		time_t ttnow = time(NULL);
		tm* tmnow = localtime(&ttnow);
#ifdef PLATFROM_WINDOWS
		std::wstring path = LocateModulePath();
		auto logpath = StrConvert::UTF8ToGbk(StrConvert::wstringToUTF8(path));
		Logger::PATH = fmt::format("{}\\pubcriberlog\\pubcriberlog_{:04}-{:02}-{:02}.log", logpath, tmnow->tm_year + 1900, tmnow->tm_mon + 1, tmnow->tm_mday);
#else
		char* cpath = getcwd(NULL, 0);
		std::string strPath = cpath;
		std::wstring path = StrConvert::UTF8Towstring(strPath);
		Logger::PATH = fmt::format("{}/pubcriberlog/pubcriberlog_{:04}-{:02}-{:02}.log", strPath, tmnow->tm_year + 1900, tmnow->tm_mon + 1, tmnow->tm_mday);
#endif
		ConfigLoader configLoader(path + L"/◊‹œﬂ≈‰÷√.xml");
		auto config = configLoader.load();
		if (config != nullptr)
		{
			m_whiteboard = make_unique<Whiteboard>(config->name, config->pubConfig, config->subConfig);
		}
		if (m_whiteboard)
			setupTopic();
	}

	DataReceiver::~DataReceiver()
	{
		m_whiteboard.reset();
	}

	void DataReceiver::publish(const std::string& topic, MessageBuilder& mb)
	{
		m_whiteboard->publish(topic, mb);
	}

	void DataReceiver::setupTopic()
	{
		auto pscanTopic = concatString(list<string>{"DCTS", "PScan", "Result"}, "#");
		m_whiteboard->registerTopic(pscanTopic, [this](MessageExtractor& me) 
			{
				onPScanResult(me); 
			});
	}

	void DataReceiver::onPScanResult(MessageExtractor& me)
	{
		zczh::zhpscan::Result result;
		if (me.deserialize(result))
		{
			if (m_irrcbs.receivePScanHandler)
				m_irrcbs.receivePScanHandler(result);
		}
	}
}