#pragma once
#include "stdint.h"

namespace DCTSDataTransport
{
#pragma pack(1)
	struct Position
	{
		double latitude;	//纬度，单位 弧度
		double longitude;   //经度，单位 弧度
		double altitude;    //海拔，单位 米
	};

	//时间戳，UTC time stamp
	struct Timestamp
	{
		uint32_t seconds = 1;  //UTC时间 自1970-1-1日零点以来的秒值
		uint32_t nanos = 2;    //本秒内的纳秒偏移
	};

	//时间范围
	struct TimeSpan
	{
		Timestamp start_time;
		Timestamp stop_time;
	};

	struct FrequencySpan
	{
		double start_freq;   //起始频率
		double stop_freq;    //终止频率
	};

	struct DataBlock
	{
		float* addr;
		size_t length;
	};

	struct Header
	{
		uint32_t sequence_number;  //顺序号，每个任务执行者维护自身的顺序号
		uint32_t sweep_count;      //当前上报周期内的扫描次数
		TimeSpan time_span;        //当前上报周期内的首段和末端采集时刻（由设备提供的时间）
		Position device_position;  //设备位置
	};

	struct View
	{
		FrequencySpan freq_span;        //频率范围
		DataBlock cur_trace;            //最近时刻曲线
		DataBlock minhold_trace;        //最大保持线
		DataBlock maxhold_trace;        //最小保持线
	};

	struct SignalSegment
	{
		char* id;                    //信号id
		uint32_t segment_seq;        //信号段号
		double   center_freq;        //中心频率，单位Hz
		double   band_width;         //带宽，单位Hz
		double   amplitude;          //幅度，单位dBm
		TimeSpan time_span6;         //信号出现的起始和终止时间
		uint32_t total_sweep;        //扫描次数
		uint32_t total_hits;         //出现次数 
	};

	struct SignalList
	{
		int32_t signalCount;    //信号数量
		SignalSegment* detected_signals;
	};

	struct PScanResult
	{
		Header header;
		View panorama_view;     //全景视角的数据
		View closeshot_view;    //近景视角的数据
		SignalList signal_list; //结果时间段内检出的信号列表
	};

	struct DeviceStatus
	{
		Position position;  //位置
		double temperature; //温度
		double humidity;    //湿度
		double velocity;    //电压
	};

	//调制类型
	enum ModulationType
	{
		UNKNOWM3 = 0,
		AM = 1,
		AMSC = 2,
		AMTC = 3,
		SSB = 4,
		DSB = 5,
		VSB = 6,
		LSB = 7,
		USB = 8,
		FM = 11,
		ASK = 21,
		ASK2 = 22,
		ASK4 = 23,
		ASK8 = 24,
		ASK16 = 25,
		FSK = 31,
		FSK2 = 32,
		FSK4 = 33,
		FSK8 = 34,
		FSK16 = 35,
		MSK = 36,
		PSK = 41,
		BPSK = 42,
		OQPSK = 43,
		QPSK = 44,
		Pi4QPSK = 45,
		PSK8 = 46,
		PSK16 = 47,
		D8PSK = 48,
		QAM = 51,
		QAM16 = 52,
		QAM32 = 53,
		QAM64 = 54,
		QAM128 = 55,
		QAM256 = 56,
		QAM512 = 57,
		QAM1024 = 58,
		CW = 70,
		Noise = 80,
	};

	//信号类型
	enum SignalType
	{
		UNKNOWM2 = 0,     //未知
		NARROW_BAND = 1, //窄带
		WIDE_BAND = 2,   //宽带
		PULSE = 3,       //脉冲
	};

	//制式类型
	enum TransmissionType
	{
		UNKNOWM1 = 0,    //未知
		GSM = 1,        //GMS
		CDMA2000 = 2,   //CDMA2000
		WCDMA = 3,      //WCDMA
		TD_SCDMA = 4,   //TD SCDM
		FourG_TDD = 5,  //4G TDD
		FourG_FDD = 6,  //4G FDD
		FiveG = 7,      //5G
	};

	//情报信息
	struct IntelligenceInformation
	{
		double   frequency;         //中心频率，单位Hz
		double   bandWidth;         //带宽，单位Hz
		double   amplitude;         //幅度，单位dBm
		double	 direction;	        //方向
		Position position;          //位置 
		TimeSpan timeSpan;          //出现的起始和终止时间
		SignalType signalType;      //信号类型
		ModulationType modType;     //调制类型
		TransmissionType transType; //制式类型
	};

#pragma pack()
}