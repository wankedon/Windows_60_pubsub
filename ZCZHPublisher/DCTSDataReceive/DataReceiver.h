#pragma once
#include "ZMsgWrap.h"
#include "node/zczh/ZhPScan.pb.h"

class Whiteboard;

namespace DCTSDataTransport
{	
	using InnerReceivePScanResultHandler = std::function<void(zczh::zhpscan::Result result)>;
	struct InnerReceiveResultCallBacks
	{
		InnerReceivePScanResultHandler receivePScanHandler = nullptr;
	};

	class DataReceiver
	{
	public:
		DataReceiver(const InnerReceiveResultCallBacks& irrcbs);
		~DataReceiver();

	public:
		void publish(const std::string& topic, MessageBuilder& mb);

	private:
		void setupTopic();
		void onPScanResult(MessageExtractor& me);

	private:
		InnerReceiveResultCallBacks m_irrcbs;
		std::unique_ptr<Whiteboard> m_whiteboard;
	};
}