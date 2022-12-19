#pragma once
#include "stdint.h"

namespace DCTSDataTransport
{
#pragma pack(1)
	struct Position
	{
		double latitude;	//γ�ȣ���λ ����
		double longitude;   //���ȣ���λ ����
		double altitude;    //���Σ���λ ��
	};

	//ʱ�����UTC time stamp
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

	struct FrequencySpan
	{
		double start_freq;   //��ʼƵ��
		double stop_freq;    //��ֹƵ��
	};

	struct DataBlock
	{
		float* addr;
		size_t length;
	};

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

	struct SignalSegment
	{
		char* id;                    //�ź�id
		uint32_t segment_seq;        //�źŶκ�
		double   center_freq;        //����Ƶ�ʣ���λHz
		double   band_width;         //������λHz
		double   amplitude;          //���ȣ���λdBm
		TimeSpan time_span6;         //�źų��ֵ���ʼ����ֹʱ��
		uint32_t total_sweep;        //ɨ�����
		uint32_t total_hits;         //���ִ��� 
	};

	struct SignalList
	{
		int32_t signalCount;    //�ź�����
		SignalSegment* detected_signals;
	};

	struct PScanResult
	{
		Header header;
		View panorama_view;     //ȫ���ӽǵ�����
		View closeshot_view;    //�����ӽǵ�����
		SignalList signal_list; //���ʱ����ڼ�����ź��б�
	};

	struct DeviceStatus
	{
		Position position;  //λ��
		double temperature; //�¶�
		double humidity;    //ʪ��
		double velocity;    //��ѹ
	};

	//��������
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

	//�ź�����
	enum SignalType
	{
		UNKNOWM2 = 0,     //δ֪
		NARROW_BAND = 1, //խ��
		WIDE_BAND = 2,   //���
		PULSE = 3,       //����
	};

	//��ʽ����
	enum TransmissionType
	{
		UNKNOWM1 = 0,    //δ֪
		GSM = 1,        //GMS
		CDMA2000 = 2,   //CDMA2000
		WCDMA = 3,      //WCDMA
		TD_SCDMA = 4,   //TD SCDM
		FourG_TDD = 5,  //4G TDD
		FourG_FDD = 6,  //4G FDD
		FiveG = 7,      //5G
	};

	//�鱨��Ϣ
	struct IntelligenceInformation
	{
		double   frequency;         //����Ƶ�ʣ���λHz
		double   bandWidth;         //������λHz
		double   amplitude;         //���ȣ���λdBm
		double	 direction;	        //����
		Position position;          //λ�� 
		TimeSpan timeSpan;          //���ֵ���ʼ����ֹʱ��
		SignalType signalType;      //�ź�����
		ModulationType modType;     //��������
		TransmissionType transType; //��ʽ����
	};

#pragma pack()
}