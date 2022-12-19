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

	//Ƶ������
	struct Spectrum
	{
		FrequencySpan freq_span;  //Ƶ�ʷ�Χ
		DataBlock trace;          //����
	};

	//�ź�����
	struct SignalSegment
	{
		double   center_freq;        //����Ƶ�ʣ���λHz
		double   band_width;         //������λHz
		double   amplitude;          //���ȣ���λdBm
		TimeSpan time_span;          //�źų��ֵ���ʼ����ֹʱ��
	};

	//�ź��б�
	struct SignalList
	{
		SignalSegment* signalAddr;  //�źŵ�ַ
		int32_t signalCount;        //�ź�����
	};

	//ȫ��ɨ����
	struct PScanResult
	{
		Spectrum spectrum;       //Ƶ������
		SignalList signalList;   //�ź��б�
	};

	//��������
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

	//�ź�����
	enum SignalType
	{
		UNKNOWM_SIGNAL = 0,//δ֪
		NARROW_BAND = 1,   //խ��
		WIDE_BAND = 2,     //���
		PULSE = 3,         //����
	};

	//����ʶ����
	struct RecognizeResult
	{
		SignalType signal_type;
		ModulationType mod_type;
		double center_freq;
		double band_width;
	};

	//��ʽ����
	enum TransmissionType
	{
		UNKNOWM_TRANS = 0, //δ֪
		GSM = 1,           //GMS
		CDMA2000 = 2,      //CDMA2000
		WCDMA = 3,         //WCDMA
		TD_SCDMA = 4,      //TD SCDM
		FourG_TDD = 5,     //4G TDD
		FourG_FDD = 6,     //4G FDD
		FiveG = 7,         //5G
	};

	//��Ƶ�������
	struct IFAnalysisResult
	{
		Spectrum spectrum;               //Ƶ������
		RecognizeResult recResult;       //����ʶ����
		TransmissionType transResult;    //��ʽʶ����
	};

	//Ŀ�귽λ
	struct TargetDirection
	{
		double	 centerFrequency;   //Ƶ��
		double	 direction;	        //Ŀ�귽�򣨵�λ���㣩
		int32_t	 confidence;	    //���Ŷ�
		float	 amplitude;	        //���ȣ���λ��dBm��
	};

	//������
	struct DirectionResult
	{
		Spectrum spectrum;       //Ƶ��
		TargetDirection target;  //Ŀ�귽λ
	};

	//��λ���
	struct PositionResult
	{
		Position position;   //Ŀ��λ��
	};


	//�豸״̬
	enum DeviceStatus
	{
		OFFLINE = 0,  //����
		ONLINE = 1,   //����
	};

	//�豸��Ϣ
	struct DeviceInformation
	{
		DeviceStatus status;//״̬
		Position position;  //λ��
		double temperature; //�¶�
		double humidity;    //ʪ��
		double velocity;    //��ѹ
	};
#pragma pack()
}