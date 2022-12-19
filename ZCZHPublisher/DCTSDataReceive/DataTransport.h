#pragma once
#include "ZMsgWrap.h"
#include "node/zczh/ZhPScan.pb.h"
#include "ComDef.h"

namespace DCTSDataTransport
{
	typedef void ReceivePScanResultHandler(const PScanResult& pscanResult, void* ptr);//接收全景数据
	struct ReceiveResultCallBacks
	{
		void* owner = nullptr;
		ReceivePScanResultHandler* receivePScanHandler = nullptr;
	};

	class DataReceiver;
	class DataConvert;

	class DataTransport
	{
	public:
		DataTransport(const ReceiveResultCallBacks& rrcbs);
		~DataTransport();

	public:
		void publish(const std::string& topic, MessageBuilder& mb);

	private:
		void onReceivePScanResult(const zczh::zhpscan::Result& result);

	private:
		ReceiveResultCallBacks m_rrcbs;
		std::unique_ptr<DataConvert> m_convert;
		std::unique_ptr<DataReceiver> m_receiver;
	};
}