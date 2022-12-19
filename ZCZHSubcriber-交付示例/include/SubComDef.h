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

	//结果头
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

	//信号描述
	struct SignalSegment
	{
		char* id;                    //信号id
		uint32_t segment_seq;        //信号段号
		double   center_freq;        //中心频率，单位Hz
		double   band_width;         //带宽，单位Hz
		double   amplitude;          //幅度，单位dBm
		TimeSpan time_span;          //信号出现的起始和终止时间
		uint32_t total_sweep;        //扫描次数
		uint32_t total_hits;         //出现次数 
	};

	//信号列表
	struct SignalList
	{
		SignalSegment* detected_signals; //起始地址
		int32_t signalCount;             //信号数量
	};

	//全景扫描结果
	struct PScanResult
	{
		Header header;          //结果头
		View panorama_view;     //全景视角的数据
		View closeshot_view;    //近景视角的数据
		SignalList signal_list; //结果时间段内检出的信号列表
	};
#pragma pack()
}