#include "pch.h"
#include "ActorBase.h"
#include "Poll.h"
#include "Logger.h"

using namespace std;
static const char TIP_FRONTEND[] = "ACTOR_FRONTEND";

Actor::Actor()
	:poll(std::make_unique<Poll>()),
	actor(nullptr),
	pipeSock(nullptr)
{
}

Actor::~Actor()
{
	stop();
	poll.reset();
}

int Actor::setTimer(size_t interval, size_t times, std::function<void()> func)
{
	return poll->setTimer(interval, times, func);
}

void Actor::killTimer(int id)
{
	poll->killTimer(id);
}

void Actor::start(std::function<void()> func)
{
	initFunc = func;
	actor = zactor_new(backendThreadFunc, this);
}

void Actor::backendThreadFunc(zsock_t* pipe, void* arg)
{
	auto* self = (Actor*)arg;
	self->startLoop(pipe);
}

void Actor::startLoop(zsock_t* pipe)
{
	pipeSock = pipe;
	poll->addReader(pipeSock, [this]() {return onPipeEvent(); });
	if (initFunc)
	{
		initFunc();
	}
	zsock_signal(pipe, 0);//��ǰ̨�����źţ�ָ����̨�߳��Ѿ���ʼִ�У�
	poll->start();
}

void Actor::stop()
{
	if (actor)
	{
		zactor_destroy(&actor);
	}
}

void Actor::bindActorMsgHandler(std::function<void(MessageExtractor&)> func)
{
	actorMsgHandler = func;
}

int Actor::sendToFrontend(MessageBuilder& builder)
{
	lock_guard<mutex> lg(mtx);
	builder.pushString(TIP_FRONTEND);
	return builder.send(actor);
}

void Actor::addReader(zsock_t* s, std::function<int()> cb)
{
	poll->addReader(s, cb);
}

int Actor::onPipeEvent()
{
	auto extractor = MessageExtractor::createFromReceivedMsg(pipeSock);
	if (extractor == nullptr)
		return 0;
	string header = extractor->popString();
	if (header == "$TERM")
		return -1;
	if (header == TIP_FRONTEND && actorMsgHandler)	//һ��������ǰ̨����Ϣ
	{
		actorMsgHandler(*extractor);
	}
	return 0;
}