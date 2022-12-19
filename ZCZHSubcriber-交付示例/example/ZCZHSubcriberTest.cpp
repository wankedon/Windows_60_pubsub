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
	//初始化库
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
	//定义调函数集合
	ResultCallBacks rcbs;
	rcbs.owner = this;
	rcbs.pscanResultHandler = SubcribePScanResult;
	//注册订阅者
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
	//注销订阅者
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
	//关闭库
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

//接收全景数据回调函数
void UseExample::SubcribePScanResult(uint32_t subcriberId, const PScanResult& pscanResult, void* ptr)
{
	UseExample* self = (UseExample*)(ptr);
	self->onPScanResult(subcriberId,pscanResult);
}

void UseExample::onPScanResult(uint32_t subcriberId, const PScanResult& pscanResult)
{
	std::cout << "subcriberId " << subcriberId << " ReceivePScanResult: " << pscanResult.header.sweep_count << std::endl;
	//全景视角的数据
	View panorama_view= pscanResult.panorama_view;
	//全景视角最近时刻曲线
	std::vector<float> panorama_view_cur(panorama_view.cur_trace.length);
	memcpy(panorama_view_cur.data(), panorama_view.cur_trace.addr, panorama_view_cur.size() * sizeof(float));
	//信号列表
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