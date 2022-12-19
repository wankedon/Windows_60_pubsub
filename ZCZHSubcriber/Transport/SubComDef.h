#pragma once
#include "stdint.h"

namespace ZCZHSubcriber
{
#pragma pack(1)
	//位置
	struct Position
	{
		double latitude;	//纬度，单位 弧度
		double longitude;   //经度，单位 弧度
		double altitude;    //海拔，单位 米
	};

	//时间戳
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

	//频率范围
	struct FrequencySpan
	{
		double start_freq;   //起始频率
		double stop_freq;    //终止频率
	};

	//数据块
	struct DataBlock
	{
		float* addr;   //起始地址
		size_t length; //长度
	};

	//频谱数据
	struct Spectrum
	{
		FrequencySpan freq_span;  //频率范围
		DataBlock trace;          //数据
	};

	//信号描述
	struct SignalSegment
	{
		double   center_freq;        //中心频率，单位Hz
		double   band_width;         //带宽，单位Hz
		double   amplitude;          //幅度，单位dBm
		TimeSpan time_span;          //信号出现的起始和终止时间
	};

	//信号列表
	struct SignalList
	{
		SignalSegment* signalAddr;  //信号地址
		int32_t signalCount;        //信号数量
	};

	//全景扫描结果
	struct PScanResult
	{
		Spectrum spectrum;       //频谱数据
		SignalList signalList;   //信号列表
	};

	//调制类型
	enum ModulationType
	{
		UNKNOWM_MOD = 0,
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
		UNKNOWM_SIGNAL = 0,//未知
		NARROW_BAND = 1,   //窄带
		WIDE_BAND = 2,     //宽带
		PULSE = 3,         //脉冲
	};

	//调制识别结果
	struct RecognizeResult
	{
		SignalType signal_type;
		ModulationType mod_type;
		double center_freq;
		double band_width;
	};

	//制式类型
	enum TransmissionType
	{
		UNKNOWM_TRANS = 0, //未知
		GSM = 1,           //GMS
		CDMA2000 = 2,      //CDMA2000
		WCDMA = 3,         //WCDMA
		TD_SCDMA = 4,      //TD SCDM
		FourG_TDD = 5,     //4G TDD
		FourG_FDD = 6,     //4G FDD
		FiveG = 7,         //5G
	};

	//定频分析结果
	struct IFAnalysisResult
	{
		Spectrum spectrum;               //频谱数据
		RecognizeResult recResult;       //调制识别结果
		TransmissionType transResult;    //制式识别结果
	};

	//目标方位
	struct TargetDirection
	{
		double	 centerFrequency;   //频率
		double	 direction;	        //目标方向（单位：°）
		int32_t	 confidence;	    //置信度
		float	 amplitude;	        //幅度（单位：dBm）
	};

	//测向结果
	struct DirectionResult
	{
		Spectrum spectrum;       //频谱
		TargetDirection target;  //目标方位
	};

	//定位结果
	struct PositionResult
	{
		Position position;   //目标位置
	};


	//设备状态
	enum DeviceStatus
	{
		OFFLINE = 0,  //离线
		ONLINE = 1,   //在线
	};

	//设备信息
	struct DeviceInformation
	{
		DeviceStatus status;//状态
		Position position;  //位置
		double temperature; //温度
		double humidity;    //湿度
		double velocity;    //电压
	};
#pragma pack()
}