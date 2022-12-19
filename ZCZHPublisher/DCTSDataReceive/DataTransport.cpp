#include "pch.h"
#include "DataTransport.h"
#include "DataReceiver.h"
#include "DataConvert.h"

namespace DCTSDataTransport
{
	using namespace std;

	DataTransport::DataTransport(const ReceiveResultCallBacks& rrcbs)
		:m_rrcbs(rrcbs),
		m_convert(make_unique<DataConvert>())
	{
		InnerReceiveResultCallBacks irrcbs{
        std::bind(&DataTransport::onReceivePScanResult, this, std::placeholders::_1)

		};
		m_receiver = make_unique<DataReceiver>(irrcbs);
	}

	DataTransport::~DataTransport()
	{

	}

	void DataTransport::publish(const std::string& topic, MessageBuilder& mb)
	{
		m_receiver->publish(topic, mb);
	}

	void DataTransport::onReceivePScanResult(const zczh::zhpscan::Result& result)
	{
		auto pscanResult = m_convert->convertPScanResult(result);
		if (m_rrcbs.receivePScanHandler)
			m_rrcbs.receivePScanHandler(pscanResult, m_rrcbs.owner);
	}
}