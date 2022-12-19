#pragma once
#include <mutex>
#include "ActorBase.h"

class StreamSource
{
public:
	StreamSource(const ZeromqLinkCfg& sourceCfg);
	~StreamSource();
public:
	int send(const MessageBuilder& mb);
private:
	zsock_t* push;
	std::timed_mutex streamLock;
	std::atomic<uint32_t> block_times;
	std::atomic<uint32_t> send_fail_times;  //����ʧ�ܴ���
	std::atomic<uint32_t> sended_message_count; //���͵���Ϣ����
};

class StreamSink : public Actor
{
public:
	StreamSink(const ZeromqLinkCfg& sinkCfg);
	~StreamSink();
public:
	void bindStreamMsgHandler(std::function<void(MessageExtractor&)> handler);
private:
	void onStreamEvent();
private:
	zsock_t* pull;
	std::function<void(MessageExtractor&)> msgHandler;
};

