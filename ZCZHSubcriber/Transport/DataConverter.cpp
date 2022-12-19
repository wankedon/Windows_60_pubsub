#include "pch.h"
#include "DataConverter.h"

namespace ZCZHSubcriber
{
	DataConverter::DataConverter()
	{

	}

	DataConverter::~DataConverter()
	{

	}

	PScanResult DataConverter::convertPScanResult(zczh::zhpscan::Result result)
	{
		PScanResult pscanResult;
		//pscanResult.header = headerConvert(result.header());
		pscanResult.spectrum = viewConvert(result.panorama_view(), m_panoTrace);
		//pscanResult.closeshot_view = viewConvert(result.closeshot_view(), m_closeTrace);
		pscanResult.signalList = signalListConvert(result.signal_list(), m_signalSegment, m_signalId);
		return pscanResult;
	}

	IFAnalysisResult DataConverter::convertIFResult(zczh::zhIFAnalysis::Result result)
	{
		IFAnalysisResult ifResult;
		spectrum::Header iheader;
		*iheader.mutable_result_from() = result.header().result_from();
		*iheader.mutable_record_id() = result.header().record_id();
		*iheader.mutable_time_span() = result.header().time_span();
		*iheader.mutable_device_position() = result.header().device_position();
		iheader.set_sequence_number(result.header().sequence_number());
		iheader.set_sweep_count(result.header().sweep_count());
		//ifResult.header = headerConvert(iheader);
		ifResult.spectrum = viewConvert(result.spectrum(), m_panoTrace);
		//ifResult.signal_list = signalListConvert(result.signal_list(), m_signalSegment, m_signalId);
		ifResult.recResult = RecConvert(result.recognize_result());
		ifResult.transResult = (TransmissionType)result.trans_result().trans_type();
		return ifResult;
	}

	DirectionResult DataConverter::convertDFResult(zczh::zhdirection::Result result)
	{
		DirectionResult dfResult;
		spectrum::Header iheader;
		*iheader.mutable_result_from() = result.header().result_from();
		*iheader.mutable_record_id() = result.header().record_id();
		*iheader.mutable_time_span() = result.header().time_span();
		*iheader.mutable_device_position() = result.header().device_position();
		iheader.set_sequence_number(result.header().sequence_number());
		iheader.set_sweep_count(result.header().sweep_count());
		//dfResult.header = headerConvert(iheader);
		dfResult.target = TargetConvert(result.target_detection());
		dfResult.spectrum = viewConvert(result.df_spectrum(), m_panoTrace);
		return dfResult;
	}

	PositionResult DataConverter::convertPosResult(zczh::zhposition::Result result)
	{
		PositionResult posResult;
		posResult.position = positionConvert(result.target_position());
		return posResult;
	}

	DeviceInformation DataConverter::convertDeviceInfo(zb::dcts::node::DeviceInfo deviceInfo)
	{
		DeviceInformation devInfo;
		devInfo.position = positionConvert(deviceInfo.position());
		for (auto physical: deviceInfo.physicals())
		{
			if (physical.unit() == "MasterControl_Temperature")
			{
				devInfo.temperature = physical.value();
			}
			if (physical.unit() == "MasterControl_Humidity")
			{
				devInfo.humidity = physical.value();
			}
			if (physical.unit() == "MasterControl_Voltage")
			{
				devInfo.velocity = physical.value();
			}
		}
		if (deviceInfo.status() == zb::dcts::node::DeviceStatus::D_IDLE)
		{
			devInfo.status = DeviceStatus::ONLINE;
		}
		if (deviceInfo.status() == zb::dcts::node::DeviceStatus::D_OFFLINE)
		{
			devInfo.status = DeviceStatus::OFFLINE;
		}
		return devInfo;
	}

	//Header DataConverter::headerConvert(const spectrum::Header& pheader)
	//{
	//	Header header;
	//	header.sequence_number = pheader.sequence_number();
	//	header.sweep_count = pheader.sweep_count();
	//	header.device_position = positionConvert(pheader.device_position());
	//	return header;
	//}

	Position DataConverter::positionConvert(const zb::dcts::Position& pos)
	{
		Position position;
		position.longitude = pos.longitude();
		position.latitude = pos.latitude();
		position.altitude = pos.altitude();
		return position;
	}

	Spectrum DataConverter::viewConvert(const spectrum::View& pView, TraceList& trace)
	{
		Spectrum view;
		view.freq_span.start_freq = pView.freq_span().start_freq();
		view.freq_span.stop_freq = pView.freq_span().stop_freq();
		trace.resize(3);
		trace[0] = { pView.cur_trace().begin(),pView.cur_trace().end() };
		trace[1] = { pView.maxhold_trace().begin(),pView.maxhold_trace().end() };
		trace[2] = { pView.minhold_trace().begin(),pView.minhold_trace().end() };
		view.trace = { trace[0].data(),trace[0].size() };
		//view.maxhold_trace = { trace[1].data(),trace[1].size() };
		//view.minhold_trace = { trace[2].data(),trace[2].size() };
		return view;
	}

	SignalList DataConverter::signalListConvert(const detection::SignalList& psignalList, SignalSegmentList& signalSegments, SignalIdList& signalId)
	{
		signalSegments.resize(psignalList.detected_signals_size());
		signalId.resize(psignalList.detected_signals_size());
		int id = 0;
		SignalSegment signalSegment;
		for (auto ds : psignalList.detected_signals())
		{
			signalId[id] = ds.id();
			//signalSegment.id = const_cast<char*>(signalId[id].c_str());
			//signalSegment.segment_seq = ds.segment_seq();
			signalSegment.center_freq = ds.center_freq().cur();
			signalSegment.band_width = ds.band_width().cur();
			signalSegment.amplitude = ds.amplitude().cur();
			//signalSegment.total_sweep = ds.total_sweep();
			//signalSegment.total_hits = ds.total_hits();
			signalSegment.time_span.start_time.seconds = ds.time_span().start_time().seconds();
			signalSegment.time_span.start_time.nanos = ds.time_span().start_time().nanos();
			signalSegment.time_span.stop_time.seconds = ds.time_span().stop_time().seconds();
			signalSegment.time_span.stop_time.nanos = ds.time_span().stop_time().nanos();
			signalSegments[id] = signalSegment;
			id++;
		}
		SignalList signalList = { signalSegments.data(),signalSegments.size() };
		return signalList;
	}

	RecognizeResult DataConverter::RecConvert(const modulation::RecognizeResult& rec)
	{
		RecognizeResult recResult;
		recResult.band_width = rec.band_width();
		recResult.center_freq = rec.center_freq();
		recResult.mod_type = (ModulationType)rec.mod_type();
		recResult.signal_type = (SignalType)rec.signal_type();
		return recResult;
	}

	TargetDirection DataConverter::TargetConvert(const zczh::zhdirection::TargetDirection& tar)
	{
		TargetDirection target;
		target.direction = tar.direction();
		target.centerFrequency = tar.center_frequency();
		target.amplitude = tar.amplitude();
		target.confidence = tar.confidence();
		return target;
	}
}