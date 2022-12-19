#include "pch.h"
#include "DataSubcriber.h"
#include "DataConverter.h"
#include "Whiteboard.h"
#include "Miscellaneous.h"
#include "Logger.h"

namespace ZCZHSubcriber
{
	using namespace std;

	DataSubcriber::DataSubcriber(const ConfigLoader::BusConfig& config, const ResultCallBacks& rcbs)
		:m_subcriberId(0),
		m_rcbs(rcbs),
		m_convert(make_unique<DataConverter>()),
		m_whiteboard(make_unique<Whiteboard>(config.name, config.pubConfig, config.subConfig))
	{
		setupTopic();
	}

	DataSubcriber::~DataSubcriber()
	{
		if (m_whiteboard)
			m_whiteboard.reset();
	}

	void DataSubcriber::setupTopic()
	{
		auto pscanTopic = concatString(list<string>{"DCTS", "PScan", "Result"}, "#");
		m_whiteboard->registerTopic(pscanTopic, [this](MessageExtractor& me) {onPScanResult(me); });
		auto ifTopic = concatString(list<string>{"DCTS", "IFScan", "Result"}, "#");
		m_whiteboard->registerTopic(ifTopic, [this](MessageExtractor& me) {onIFScanResult(me); });
		auto dfTopic = concatString(list<string>{"DCTS", "DF", "Result"}, "#");
		m_whiteboard->registerTopic(dfTopic, [this](MessageExtractor& me) {onDFResult(me); });
		auto posTopic = concatString(list<string>{"DCTS", "Pos", "Result"}, "#");
		m_whiteboard->registerTopic(posTopic, [this](MessageExtractor& me) {onPosResult(me); });
		auto deviceTopic = concatString(list<string>{"DCTS", "Device", "Info"}, "#");
		m_whiteboard->registerTopic(deviceTopic, [this](MessageExtractor& me) {onDeviceStatus(me); });
	}

	void DataSubcriber::onPScanResult(MessageExtractor& me)
	{
		zczh::zhpscan::Result result;
		if (me.deserialize(result) && m_rcbs.pscanResultHandler)
		{
			LOG("Subcribe PScanResult {}", result.header().sequence_number());
			auto pscanResult = m_convert->convertPScanResult(result);
			m_rcbs.pscanResultHandler(m_subcriberId, pscanResult, m_rcbs.owner);
		}
	}

	void DataSubcriber::onIFScanResult(MessageExtractor& me)
	{
		zczh::zhIFAnalysis::Result result;
		if (me.deserialize(result) && m_rcbs.ifResultHandler)
		{
			LOG("Subcribe IFResult {}", result.header().sequence_number());
			auto ifResult = m_convert->convertIFResult(result);
			m_rcbs.ifResultHandler(m_subcriberId, ifResult, m_rcbs.owner);
		}
	}

	void DataSubcriber::onDFResult(MessageExtractor& me)
	{
		zczh::zhdirection::Result result;
		if (me.deserialize(result) && m_rcbs.dfResultHandler)
		{
			LOG("Subcribe DFResult {}", result.header().sequence_number());
			auto dfResult = m_convert->convertDFResult(result);
			m_rcbs.dfResultHandler(m_subcriberId, dfResult, m_rcbs.owner);
		}
	}

	void DataSubcriber::onPosResult(MessageExtractor& me)
	{
		zczh::zhposition::Result result;
		if (me.deserialize(result) && m_rcbs.posResultHandler)
		{
			LOG("Subcribe PosResult");
			auto posResult = m_convert->convertPosResult(result);
			m_rcbs.posResultHandler(m_subcriberId, posResult, m_rcbs.owner);
		}
	}

	void DataSubcriber::onDeviceStatus(MessageExtractor& me)
	{
		zb::dcts::node::DeviceInfo deviceInfo;
		if (me.deserialize(deviceInfo) && m_rcbs.deviceInfoHandler)
		{
			LOG("Subcribe DeviceInfo");
			auto devInfo = m_convert->convertDeviceInfo(deviceInfo);
			m_rcbs.deviceInfoHandler(m_subcriberId, devInfo, m_rcbs.owner);
		}
	}

	void DataSubcriber::release()
	{
		if (m_whiteboard)
			m_whiteboard.reset();
		if (m_convert)
			m_convert.reset();
	}
}