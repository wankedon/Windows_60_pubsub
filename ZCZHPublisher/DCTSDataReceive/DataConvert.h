#pragma once
#include "node/zczh/ZhPScan.pb.h"
#include "ComDef.h"
namespace DCTSDataTransport
{
	class DataConvert
	{
	public:
		DataConvert();
		~DataConvert();

	public:
		PScanResult convertPScanResult(zczh::zhpscan::Result result);

	};
}