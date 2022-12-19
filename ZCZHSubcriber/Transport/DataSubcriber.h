#pragma once
#include "ZMsgWrap.h"
#include "node/zczh/ZhPScan.pb.h"
#include "node/zczh/ZhIFAnalysis.pb.h"
#include "node/zczh/ZhDirection.pb.h"
#include "node/zczh/ZhPosition.pb.h"
#include "ZCZHSubcriber.h"
#include "ConfigLoader.h"

class Whiteboard;
namespace ZCZHSubcriber
{
	class DataConverter;
	class DataSubcriber
	{
	public:
		DataSubcriber(const ConfigLoader::BusConfig& config, const ResultCallBacks& rcbs);
		~DataSubcriber();
	public:
		void setSubscriberId(const uint32_t& subcriberId) { m_subcriberId = subcriberId; }
		void release();
	private:
		void setupTopic();
		void onPScanResult(MessageExtractor& me);
		void onIFScanResult(MessageExtractor& me);
		void onDFResult(MessageExtractor& me);
		void onPosResult(MessageExtractor& me);
		void onDeviceStatus(MessageExtractor& me);
	private:
		uint32_t m_subcriberId;
		ResultCallBacks m_rcbs;
		std::unique_ptr<DataConverter> m_convert;
		std::unique_ptr<Whiteboard> m_whiteboard;
	};
}