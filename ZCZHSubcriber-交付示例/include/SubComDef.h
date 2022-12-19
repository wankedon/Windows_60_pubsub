#pragma once
#include "stdint.h"

namespace ZCZHSubcriber
{
#pragma pack(1)
	//λ��
	struct Position
	{
		double latitude;	//γ�ȣ���λ ����
		double longitude;   //���ȣ���λ ����
		double altitude;    //���Σ���λ ��
	};

	//ʱ���
	struct Timestamp
	{
		uint32_t seconds = 1;  //UTCʱ�� ��1970-1-1�������������ֵ
		uint32_t nanos = 2;    //�����ڵ�����ƫ��
	};

	//ʱ�䷶Χ
	struct TimeSpan
	{
		Timestamp start_time;
		Timestamp stop_time;
	};

	//Ƶ�ʷ�Χ
	struct FrequencySpan
	{
		double start_freq;   //��ʼƵ��
		double stop_freq;    //��ֹƵ��
	};

	//���ݿ�
	struct DataBlock
	{
		float* addr;   //��ʼ��ַ
		size_t length; //����
	};

	//���ͷ
	struct Header
	{
		uint32_t sequence_number;  //˳��ţ�ÿ������ִ����ά�������˳���
		uint32_t sweep_count;      //��ǰ�ϱ������ڵ�ɨ�����
		TimeSpan time_span;        //��ǰ�ϱ������ڵ��׶κ�ĩ�˲ɼ�ʱ�̣����豸�ṩ��ʱ�䣩
		Position device_position;  //�豸λ��
	};

	struct View
	{
		FrequencySpan freq_span;        //Ƶ�ʷ�Χ
		DataBlock cur_trace;            //���ʱ������
		DataBlock minhold_trace;        //��󱣳���
		DataBlock maxhold_trace;        //��С������
	};

	//�ź�����
	struct SignalSegment
	{
		char* id;                    //�ź�id
		uint32_t segment_seq;        //�źŶκ�
		double   center_freq;        //����Ƶ�ʣ���λHz
		double   band_width;         //������λHz
		double   amplitude;          //���ȣ���λdBm
		TimeSpan time_span;          //�źų��ֵ���ʼ����ֹʱ��
		uint32_t total_sweep;        //ɨ�����
		uint32_t total_hits;         //���ִ��� 
	};

	//�ź��б�
	struct SignalList
	{
		SignalSegment* detected_signals; //��ʼ��ַ
		int32_t signalCount;             //�ź�����
	};

	//ȫ��ɨ����
	struct PScanResult
	{
		Header header;          //���ͷ
		View panorama_view;     //ȫ���ӽǵ�����
		View closeshot_view;    //�����ӽǵ�����
		SignalList signal_list; //���ʱ����ڼ�����ź��б�
	};
#pragma pack()
}