#pragma once

#ifdef __cplusplus
extern "C" {
#endif
// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� FASTFFT_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// FASTFFT_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef FASTFFT_EXPORTS
#define FASTFFT_API __declspec(dllexport)
#else
#define FASTFFT_API __declspec(dllimport)
#endif

typedef enum WINDOW_SHAPE
{
	WS_RECT,
	WS_HANNING,
	WS_HAMMING,
	WS_BLACKMAN,
	WS_KAISER,
	WS_GUASS,
	WS_SIN,
}WINDOW_SHAPE;

typedef enum _FFTDataType
{
	FDT_TIME,
	FDT_FREQ,
}FFTDataType;

FASTFFT_API uint32_t createFFTer(int fftPoints);
FASTFFT_API void deleteFFTer(uint32_t handle);
FASTFFT_API void addWindow(uint32_t handle, WINDOW_SHAPE win);
FASTFFT_API void FFTInput(uint32_t handle, float* src, FFTDataType dataType);
FASTFFT_API void FFTTransTo(uint32_t handle, FFTDataType dataType);
FASTFFT_API void FFTHighResTrans(uint32_t handle, double sampleRate, double freqOffset, double bw);	//time to freq convertion in high resolution
FASTFFT_API void FFTOutput(uint32_t handle, float* dst, FFTDataType dataType);

#ifdef __cplusplus
}
#endif