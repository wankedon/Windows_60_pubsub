#include <iostream>
#include "ZCZHSubcriber.h"
#include <vector>

using namespace std;
using namespace ZCZHSubcriber;

class UseExample
{
public:
	UseExample();
	~UseExample();

private:
	static void SubcribePScanResult(uint32_t subcriberId, const PScanResult& pscanResult, void* ptr);
	void onPScanResult(uint32_t subcriberId, const PScanResult& pscanResult);

private:
	int m_id;
};

UseExample::UseExample()
{
	//��ʼ����
	auto erro = InitLib();
	if (erro == ZCZH_ERR_NONE)
	{
		std::cout << "InitLib sucessed" << std::endl;
	}
	else
	{
		std::cout << "InitLib failed erroCode: " << erro << std::endl;
		return;
	}
	//�������������
	ResultCallBacks rcbs;
	rcbs.owner = this;
	rcbs.pscanResultHandler = SubcribePScanResult;
	//ע�ᶩ����
	erro = RegisterSubscriber(&m_id, rcbs);
	if (erro == ZCZH_ERR_NONE)
	{
		std::cout << "RegisterSubscriber sucessed Id " << m_id << std::endl;
		std::cout << "Start Receive ZCZHData ..." << std::endl;
	}
	else
	{
		std::cout << "RegisterSubscriber failed erroCode: " << erro << std::endl;
		return;
	}
}

UseExample::~UseExample()
{
	//ע��������
	auto erro = UnregisterSubscriber(m_id);
	if (erro == ZCZH_ERR_NONE)
	{
		std::cout << "UnregisterSubscriber " << m_id << " sucessed" << std::endl;
	}
	else
	{
		std::cout << "UnregisterSubscriber failed erroCode: " << erro << std::endl;
		return;
	}
	//�رտ�
	erro = DeInitLib();
	if (erro == ZCZH_ERR_NONE)
	{
		std::cout << "DeInitLib sucessed" << std::endl;
	}
	else
	{
		std::cout << "DeInitLib failed erroCode: " << erro << std::endl;
		return;
	}
}

//����ȫ�����ݻص�����
void UseExample::SubcribePScanResult(uint32_t subcriberId, const PScanResult& pscanResult, void* ptr)
{
	UseExample* self = (UseExample*)(ptr);
	self->onPScanResult(subcriberId,pscanResult);
}

void UseExample::onPScanResult(uint32_t subcriberId, const PScanResult& pscanResult)
{
	std::cout << "subcriberId " << subcriberId << " ReceivePScanResult: " << pscanResult.header.sweep_count << std::endl;
	//ȫ���ӽǵ�����
	View panorama_view= pscanResult.panorama_view;
	//ȫ���ӽ����ʱ������
	std::vector<float> panorama_view_cur(panorama_view.cur_trace.length);
	memcpy(panorama_view_cur.data(), panorama_view.cur_trace.addr, panorama_view_cur.size() * sizeof(float));
	//�ź��б�
	SignalList signal_list = pscanResult.signal_list;
	std::vector<SignalSegment> signalList(signal_list.signalCount);
	memcpy(signalList.data(), signal_list.detected_signals, signalList.size() * sizeof(SignalSegment));
}

int main()
{
	UseExample useExample;
	system("pause");
	return 0;
}