#pragma once
#include "node/zczh/ZhPScan.pb.h"
#include "node/zczh/ZhIFAnalysis.pb.h"
#include "node/zczh/ZhDirection.pb.h"
#include "node/zczh/ZhPosition.pb.h"
#include "SubComDef.h"

namespace ZCZHSubcriber
{
	using TraceList = std::vector<std::vector<float>>;
	using SignalIdList = std::vector<std::string>;
	using SignalSegmentList = std::vector<SignalSegment>;
	class DataConverter
	{
	public:
		DataConverter();
		~DataConverter();
	public:
		PScanResult convertPScanResult(zczh::zhpscan::Result result);
		IFAnalysisResult convertIFResult(zczh::zhIFAnalysis::Result result);
		DirectionResult convertDFResult(zczh::zhdirection::Result result);
		PositionResult convertPosResult(zczh::zhposition::Result result);
		DeviceInformation convertDeviceInfo(zb::dcts::node::DeviceInfo deviceInfo);
	private:
		//Header headerConvert(const spectrum::Header& pheader);
		Position positionConvert(const zb::dcts::Position& pos);
		Spectrum viewConvert(const spectrum::View& pView, TraceList& trace);
		SignalList signalListConvert(const detection::SignalList& psignalList, SignalSegmentList& signalSegments, SignalIdList& signalId);
		RecognizeResult RecConvert(const modulation::RecognizeResult& rec);
		TargetDirection TargetConvert(const zczh::zhdirection::TargetDirection& tar);
	
	private:
		TraceList m_panoTrace;
		TraceList m_closeTrace;
		SignalIdList m_signalId;
		SignalSegmentList m_signalSegment;
	};
}