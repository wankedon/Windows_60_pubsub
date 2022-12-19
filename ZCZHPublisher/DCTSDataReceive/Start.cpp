#include "pch.h"
#include <iostream>
#include "DataTransport.h"
#include "Miscellaneous.h"
#include "dcts.pb.h"
#include "node/zczh/ZhPScan.pb.h"
#include "node/zczh/ZhIFAnalysis.pb.h"
#include "node/zczh/ZhDirection.pb.h"
#include "node/zczh/ZhPosition.pb.h"
#include "SubPub.h"
#include "ComDef.h"

using namespace std;
using namespace DCTSDataTransport;

zb::dcts::Position createPosition()
{
	zb::dcts::Position pos;
	pos.set_longitude(120.1691);//120.1691 * 3.14159 / 180
	pos.set_latitude(35.97); //35.97 * 3.14159 / 180
	pos.set_altitude(87.4);
	return pos;
}

spectrum::View createView()
{
	spectrum::View view;
	view.mutable_freq_span()->set_start_freq(200e6);
	view.mutable_freq_span()->set_stop_freq(500e6);
	vector<float> cur_trace(800, 2.5);
	view.mutable_cur_trace()->CopyFrom({ cur_trace.begin(),cur_trace.end() });
	vector<float> maxhold_trace(800, 3.5);
	view.mutable_maxhold_trace()->CopyFrom({ maxhold_trace.begin(),maxhold_trace.end() });
	vector<float> minhold_trace(800, 1.5);
	view.mutable_minhold_trace()->CopyFrom({ minhold_trace.begin(),minhold_trace.end() });
	return view;
}

detection::SignalList createSignalList(int sweepCount)
{
	vector<detection::SignalSegment> signalSegment(2);
	int i = 1;
	for (auto& ss : signalSegment)
	{
		ss.set_id(std::to_string(i));
		ss.set_segment_seq(i);
		ss.mutable_center_freq()->set_cur(i * 100e6);
		ss.mutable_band_width()->set_cur(20e3);
		ss.mutable_amplitude()->set_cur(i * 100e6);
		ss.set_total_sweep(sweepCount);
		ss.set_total_hits(1);
		ss.mutable_time_span()->mutable_start_time()->set_seconds(222);
		ss.mutable_time_span()->mutable_start_time()->set_nanos(333);
		ss.mutable_time_span()->mutable_stop_time()->set_seconds(444);
		ss.mutable_time_span()->mutable_stop_time()->set_nanos(555);
		i++;
	}
	detection::SignalList signalList;
	signalList.mutable_detected_signals()->CopyFrom({ signalSegment.begin(),signalSegment.end() });
	vector<string> signalid = { "111256","227886" };
	signalList.mutable_merged_signal_id()->CopyFrom({ signalid.begin(),signalid.end() });
	return signalList;
}


zczh::zhpscan::Result createPScanResult(int sweepCount)
{
	spectrum::Header header;
	header.set_sequence_number(sweepCount);
	header.set_sweep_count(sweepCount);
	*header.mutable_device_position()= createPosition();
	//spectrum::View panorama_view;
	//panorama_view.mutable_freq_span()->set_start_freq(200e6);
	//panorama_view.mutable_freq_span()->set_stop_freq(500e6);
	//vector<float> cur_trace(800, 2.5);
	//panorama_view.mutable_cur_trace()->CopyFrom({ cur_trace.begin(),cur_trace.end() });
	//vector<float> maxhold_trace(800, 3.5);
	//panorama_view.mutable_maxhold_trace()->CopyFrom({ maxhold_trace.begin(),maxhold_trace.end() });
	//vector<float> minhold_trace(800, 1.5);
	//panorama_view.mutable_minhold_trace()->CopyFrom({ minhold_trace.begin(),minhold_trace.end() });
	//vector<detection::SignalSegment> signalSegment(2);
	//int i = 1;
	//for (auto& ss : signalSegment)
	//{
	//	ss.set_id(std::to_string(i));
	//	ss.set_segment_seq(i);
	//	ss.mutable_center_freq()->set_cur(i * 100e6);
	//	ss.mutable_band_width()->set_cur(20e3);
	//	ss.mutable_amplitude()->set_cur(i * 100e6);
	//	ss.set_total_sweep(sweepCount);
	//	ss.set_total_hits(1);
	//	i++;
	//}
	//detection::SignalList signalList;
	//signalList.mutable_detected_signals()->CopyFrom({ signalSegment.begin(),signalSegment.end()});
	//vector<string> signalid = {"111256","227886"};
	//signalList.mutable_merged_signal_id()->CopyFrom({ signalid.begin(),signalid.end() });
	zczh::zhpscan::Result result;
	*result.mutable_header() = header;
	*result.mutable_panorama_view() = createView();
	*result.mutable_closeshot_view() = createView();
	*result.mutable_signal_list() = createSignalList(sweepCount);
	return result;
}


zczh::zhIFAnalysis::Result createIFResult(int sweepCount)
{
	zczh::zhIFAnalysis::Header header;
	header.set_sequence_number(sweepCount);
	header.set_sweep_count(sweepCount);
	*header.mutable_device_position() = createPosition();
	zczh::zhIFAnalysis::Result result;
	*result.mutable_header() = header;
	*result.mutable_spectrum() = createView();
	*result.mutable_signal_list() = createSignalList(sweepCount);
	modulation::RecognizeResult recognize_result;
	recognize_result.set_center_freq(200e6);
	recognize_result.set_band_width(20e6);
	recognize_result.set_mod_type(zb::dcts::node::modulation::ModType::FM);
	recognize_result.set_signal_type(zb::dcts::node::modulation::SigType::WIDE_BAND);
	*result.mutable_recognize_result() = recognize_result;
	result.mutable_trans_result()->set_trans_type(zczh::zhIFAnalysis::TransmissionType::WCDMA);
	vector<int32_t> pcm1(800, 2.5);
	zczh::zhIFAnalysis::AudioBlock blocl1;
	blocl1.mutable_pcm_bitstream()->CopyFrom({ pcm1.begin(),pcm1.end() });
	vector<int32_t> pcm2(800, 3.5);
	zczh::zhIFAnalysis::AudioBlock blocl2;
	blocl2.mutable_pcm_bitstream()->CopyFrom({ pcm2.begin(),pcm2.end() });
	zczh::zhIFAnalysis::AudioResult audioResult;
	*audioResult.add_blocks() = blocl1;
	*audioResult.add_blocks() = blocl2;
	*result.mutable_audio_result() = audioResult;
	return result;
}

zczh::zhdirection::Result createDFResult(int sweepCount)
{
	zczh::zhdirection::Header header;
	header.set_sequence_number(sweepCount);
	header.set_sweep_count(sweepCount);
	*header.mutable_device_position() = createPosition();
	zczh::zhdirection::TargetDirection target;
	target.set_center_frequency(200E6);
	target.set_direction(135);
	target.set_confidence(1);
	target.set_amplitude(-80);
	zczh::zhdirection::Result result;
	*result.mutable_header() = header;
	*result.mutable_df_spectrum() = createView();
	*result.mutable_target_detection() = target;
	return result;
}

zczh::zhposition::Result createPosResult()
{
	zczh::zhposition::Result result;
	*result.mutable_target_position()= createPosition();
	return result;
}

Physical generalPhysical(std::string module, double value, Physical::Type type)
{
	Physical physical;
	physical.set_type(type);
	physical.set_value(value);
	physical.set_unit(module);
	return physical;
}


std::vector<Physical> physical()
{
	std::vector<Physical> physicalList;
	physicalList.emplace_back(generalPhysical("MasterControl_Temperature", 66, Physical::Type::Physical_Type_TEMPERATURE));
	physicalList.emplace_back(generalPhysical("MasterControl_Humidity", 15, Physical::Type::Physical_Type_HUMIDITY));
	physicalList.emplace_back(generalPhysical("MasterControl_Voltage", 12, Physical::Type::Physical_Type_VELOCITY));
	return physicalList;
}

zb::dcts::node::DeviceInfo createDeviceInfo()
{
	zb::dcts::node::DeviceInfo info;
	info.set_status(zb::dcts::node::DeviceStatus::D_IDLE);
	*info.mutable_position()= createPosition();
	*info.add_physicals() = generalPhysical("MasterControl_Temperature", 66, Physical::Type::Physical_Type_TEMPERATURE);
	*info.add_physicals() = generalPhysical("MasterControl_Humidity", 15, Physical::Type::Physical_Type_HUMIDITY);
	*info.add_physicals() = generalPhysical("MasterControl_Voltage", 12, Physical::Type::Physical_Type_VELOCITY);
	return info;
}

void SendPScanResult(const shared_ptr<DataTransport>& trans, int sweepCount)
{
	auto pubTopic = concatString(list<string>{"DCTS", "PScan", "Result"}, "#");
	zczh::zhpscan::Result result = createPScanResult(sweepCount);
	MessageBuilder mb;
	mb.serializeToTop(result);
	trans->publish(pubTopic, mb);
	std::cout << "Send PScanResult " << sweepCount << std::endl;
}

void SendIFResult(const shared_ptr<DataTransport>& trans, int sweepCount)
{
	auto pubTopic = concatString(list<string>{"DCTS", "IFScan", "Result"}, "#");
	zczh::zhIFAnalysis::Result result = createIFResult(sweepCount);
	MessageBuilder mb;
	mb.serializeToTop(result);
	trans->publish(pubTopic, mb);
	std::cout << "Send IFScanResult " << sweepCount << std::endl;
}

void SendDFResult(const shared_ptr<DataTransport>& trans, int sweepCount)
{
	auto pubTopic = concatString(list<string>{"DCTS", "DF", "Result"}, "#");
	zczh::zhdirection::Result result = createDFResult(sweepCount);
	MessageBuilder mb;
	mb.serializeToTop(result);
	trans->publish(pubTopic, mb);
	std::cout << "Send DFResult " << sweepCount << std::endl;
}

void SendPosResult(const shared_ptr<DataTransport>& trans, int sweepCount)
{
	auto pubTopic = concatString(list<string>{"DCTS", "Pos", "Result"}, "#");
	zczh::zhposition::Result result = createPosResult();
	MessageBuilder mb;
	mb.serializeToTop(result);
	trans->publish(pubTopic, mb);
	std::cout << "Send PosResult " << sweepCount << std::endl;
}

void SendDeviceStatus(const shared_ptr<DataTransport>& trans, int sweepCount)
{
	auto pubTopic = concatString(list<string>{"DCTS", "Device", "Info"}, "#");
	auto devinfo = createDeviceInfo();
	MessageBuilder mb;
	mb.serializeToTop(devinfo);
	trans->publish(pubTopic, mb);
	std::cout << "Send DeviceInfo " << sweepCount << std::endl;
}

void SendIntelligence(const shared_ptr<DataTransport>& trans, int sweepCount)
{
	auto pubTopic = concatString(list<string>{"DCTS", "Intelligence", "Information"}, "#");
	IntelligenceInformation info;
	info.frequency = 200E6;
	info.bandWidth = 20e6;
	info.amplitude = -30;
	info.direction = 125;
	info.position.longitude = 120.1691;
	info.position.latitude = 35.97;
	info.position.altitude = 87.4;
	info.timeSpan.start_time.seconds = 1;
	info.timeSpan.start_time.nanos = 2;
	info.timeSpan.stop_time.seconds = 5;
	info.timeSpan.stop_time.nanos = 6;
	info.signalType = SignalType::WIDE_BAND;
	info.modType = ModulationType::ASK;
	info.transType = TransmissionType::GSM;
	MessageBuilder mb;
	mb.add(info);
	trans->publish(pubTopic, mb);
	std::cout << "Send Intelligence " << sweepCount << std::endl;
}

int main()
{
	ReceiveResultCallBacks rrcbs;
	rrcbs.owner = (void*)123;
	rrcbs.receivePScanHandler = nullptr;
	auto trans = make_shared<DataTransport>(rrcbs);
	int i = 0;
	while (true)
	{
#ifdef PLATFROM_WINDOWS
		Sleep(500);
#else
		sleep(1);
#endif
		SendPScanResult(trans,i); //发送全景数据
		SendIFResult(trans, i);   //发送定频数据
		SendDFResult(trans, i);   //发送测向数据
		SendPosResult(trans, i);  //发送定位数据
		SendDeviceStatus(trans,i);  //发送设备状态
		//SendIntelligence(trans, i); //发送情报信息
		i++;
	}
	trans.reset();
	zsys_shutdown();
	return 0;
}