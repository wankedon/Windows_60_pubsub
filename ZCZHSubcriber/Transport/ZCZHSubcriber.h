#pragma once

#ifdef PLATFROM_WINDOWS
#ifdef ZCZHSUBCRIBER_EXPORTS
#define ZCZHSUBCRIBER_API __declspec(dllexport)
#else
#define ZCZHSUBCRIBER_API __declspec(dllimport)
#endif
#else
#define ZCZHSUBCRIBER_API
#endif

#include "SubComDef.h" //结构体声明头文件
namespace ZCZHSubcriber
{
	/** 错误码
	*  @note:  动态库接口函数返回值
	*/
	enum ZCZH_Error
	{
		ZCZH_ERR_NONE = 0,                //无错误
		ZCZH_ERR_INIT = -1,               //初始化错误
		ZCZH_ERR_INVALID_HANDLE = -2,     //句柄无效
		ZCZH_ERR_READ_CONFIG = -3,        //配置文件读取错误
		ZCZH_ERR_NO_HANDLE_RESOURCE = -4, //无句柄资源
	};

	/** 接收全景数据回调函数
	*  @param:  const uint32_t subcriberId，订阅者Id，标识数据隶属于哪个订阅者
	*  @param:  const PScanResult& pscanResult，接收到的全景数据
	*  @param:  void* ptr，不定类型指针，访问类成员函数时使用
	*  @note:   用于接收全景数据
	*　　　　　  该函数由用户预先定义，赋值给回调函数集合即可，当动态库接收到全景数据后便会触发该函数
	*/
	typedef void PScanResultHandler(const uint32_t subcriberId, const PScanResult& pscanResult, void* ptr);

	/** 接收定频分析数据回调函数
    *  @param:  const uint32_t subcriberId，订阅者Id，标识数据隶属于哪个订阅者
	*  @param:  const IFAnalysisResult& ifResult，接收到的定频分析数据
	*  @param:  void* ptr，不定类型指针，访问类成员函数时使用
	*  @note:   用于接收定频分析数据
	*　　　　　  该函数由用户预先定义，赋值给回调函数集合即可，当动态库接收到定频分析数据后便会触发该函数
	*/
	typedef void IFAnalysisResultHandler(const uint32_t subcriberId, const IFAnalysisResult& ifResult, void* ptr);

	/** 接收测向数据回调函数
	*  @param:  const uint32_t subcriberId，订阅者Id，标识数据隶属于哪个订阅者
	*  @param:  const DirectionResult& dfResult，接收到的测向数据
	*  @param:  void* ptr，不定类型指针，访问类成员函数时使用
	*  @note:   用于接收测向数据
	*　　　　　  该函数由用户预先定义，赋值给回调函数集合即可，当动态库接收到测向数据后便会触发该函数
	*/
	typedef void DirectionResultHandler(const uint32_t subcriberId, const DirectionResult& dfResult, void* ptr);

	/** 接收定位数据回调函数
	*  @param:  const uint32_t subcriberId，订阅者Id，标识数据隶属于哪个订阅者
	*  @param:  const PositionResult& posResult，接收到的定位数据
	*  @param:  void* ptr，不定类型指针，访问类成员函数时使用
	*  @note:   用于接收定位数据
	*　　　　　  该函数由用户预先定义，赋值给回调函数集合即可，当动态库接收到定位数据后便会触发该函数
	*/
	typedef void PositionResultHandler(const uint32_t subcriberId, const PositionResult& posResult, void* ptr);

	/** 接收设备状态回调函数
	*  @param:  const uint32_t subcriberId，订阅者Id，标识数据隶属于哪个订阅者
	*  @param:  const DeviceStatus& deviceStatus，接收到的设备状态
	*  @param:  void* ptr，不定类型指针，访问类成员函数时使用
	*  @note:   用于接收设备状态
	*　　　　　  该函数由用户预先定义，赋值给回调函数集合即可，当动态库接收到设备状态后便会触发该函数
	*/
	typedef void DeviceInfoHandler(const uint32_t subcriberId, const DeviceInformation& deviceStatus, void* ptr);

	/** 回调函数集合
	*  @param:  void* owner 不定类型指针，访问类成员函数时使用，初值为nullptr，通常赋值为this指针即可
	*  @param:  pscanResultHandler* 接收全景数据回调函数指针，初值为nullptr，
	*  @note:   包含一系列接收数据的回调函数
	*　　　　　  该结构体由用户预先定义，通过注册订阅者接口将其传递给动态库即可
	*/
	struct ResultCallBacks
	{
		void* owner = nullptr;
		PScanResultHandler* pscanResultHandler = nullptr;
		IFAnalysisResultHandler* ifResultHandler = nullptr;
		DirectionResultHandler* dfResultHandler = nullptr;
		PositionResultHandler* posResultHandler = nullptr;
		DeviceInfoHandler* deviceInfoHandler = nullptr;
	};

	/** 初始化库
	*  @return: ZCZH_Error 初始化库是否成功
	*  @note:   用于获取调用库函数时所需的资源
	*　　　　　  该函数为使用动态库时第一个调用的函数，必须调用且仅调用一次
	*/
	ZCZHSUBCRIBER_API ZCZH_Error InitLib();

	/** 注册订阅者
	*  @param:  int* subcriberId 输出参数，注册订阅者成功后会返回一个订阅者Id
	*  @param:  const ResultCallBacks& rcbs 回调函数集合，用于接收数据
	*  @return: ZCZH_Error 注册订阅是否成功
	*  @note:   通过该接口注册订阅者并将接收数据的回调函数传递给动态库  
	*           该函数可调用多次，每调用一次就会注册一个订阅者
	*/
	ZCZHSUBCRIBER_API ZCZH_Error RegisterSubscriber(int* subcriberId, const ResultCallBacks& rcbs);

	/** 注销订阅者
	*  @param:  int subcriberId 输入参数，该参数为注册订阅者时返回的那个Id
	*  @return: ZCZH_Error 注销订阅者是否成功
	*  @note:   通过该接口注销一个指定Id的订阅者，该函数可调用多次，每调用一次就会注销一个订阅者　
	*/
	ZCZHSUBCRIBER_API ZCZH_Error UnregisterSubscriber(int subcriberId);

	/** 关闭库
	*  @return: ZCZH_Error 关闭库是否成功
	*  @note:   用于释放通过初始化库函数获得的资源
	*　　　　　  该函数为使用动态库时最后一个调用的函数，仅调用一次，调用此函数之后，除了InitLib()，不能再调用其它接口函数
	*/
	ZCZHSUBCRIBER_API ZCZH_Error DeInitLib();
}