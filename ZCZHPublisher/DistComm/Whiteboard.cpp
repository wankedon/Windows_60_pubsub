#include "pch.h"
#include "Whiteboard.h"
#include "SharedInfo.h"
#include "Logger.h"
#include "Miscellaneous.h"

using namespace std;

Whiteboard::Whiteboard(const std::string& msgBusName, const ZeromqLinkCfg& msgBusPubConf, const ZeromqLinkCfg& msgBusSubConf)
	:address(genUUID()),
	 sub(make_unique<Subscriber>(msgBusPubConf)),	//������Ϣ���ߵķ����˿�
	 pub(make_unique<Publisher>(msgBusSubConf))		//������Ϣ���ߵĶ��Ķ˿�
{
	sub->subTopic(msgBusName, [this](MessageExtractor& me) {onBusMessage(me); });	//������Ϣ���ߵ�����
}

Whiteboard::~Whiteboard()
{
	sub.reset();
	pub.reset();
}

bool Whiteboard::isPeerMsg(const std::string& msgAddress)
{
	return (!msgAddress.empty() && msgAddress != address);
}

void Whiteboard::registerTopic(const std::string& topic, std::shared_ptr<ISharedInfo> sinfo)
{
	sub->subTopic(topic, 
	[this, sinfo](MessageExtractor& me) {
		if (extractHeaderAndCheck(me)){
			sinfo->synchronize(me);
		}
	});
}

void Whiteboard::registerTopic(const std::string& topic, std::function<void(MessageExtractor&)> func)
{
	sub->subTopic(topic,
		[this, func](MessageExtractor& me) {
			if (extractHeaderAndCheck(me)) {
				func(me);
			}
		});
}

void Whiteboard::unregisterTopic(const std::string& topic)
{
	sub->unsubTopic(topic);
}

void Whiteboard::publish(const std::string& topic, std::shared_ptr<ISharedInfo> sinfo, const TimerParam* timer)
{
	if (timer == nullptr)
	{
		sendMsgByPub(topic, sinfo);
	}
	else
	{
		auto sendFunc = [this, topic, sinfo]() {sendMsgByPub(topic, sinfo); };
		sub->setTimer(timer->interval_in_ms(), timer->times(), sendFunc);	//����sub�Ķ�ʱ�������Է���
	}
}

void Whiteboard::publish(const std::string& topic, MessageBuilder& mb)
{
	mb.pushString(address).pushString(topic);
	pub->broadcast(mb);
}

void Whiteboard::sendMsgByPub(const std::string& topic, std::shared_ptr<ISharedInfo> sinfo)
{
	auto mb = sinfo->geneMessage();
	if (mb)
	{
		(*mb).pushString(address).pushString(topic);
		pub->broadcast(*mb);
	}
}

bool Whiteboard::extractHeaderAndCheck(MessageExtractor& me)
{
	return isPeerMsg(me.popString());
}

void Whiteboard::onBusMessage(MessageExtractor& me)
{
	auto info = me.popString();
	if (!info.empty())
	{
		CLOG("broker said '{}'.", info);
	}
}
