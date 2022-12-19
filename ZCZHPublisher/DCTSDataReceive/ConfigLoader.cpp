#include "pch.h"
#include "ConfigLoader.h"
#include "Miscellaneous.h"
#include "StringConv.h"
#include "Logger.h"

using namespace std;

ConfigLoader::ConfigLoader(const std::wstring& fp)
	:filePath(fp)
{
}

std::unique_ptr<ConfigLoader::BusConfig> ConfigLoader::load()
{
	pugi::xml_document doc;
	if (!doc.load_file(filePath.c_str()))
	{
		LOG("configuration load failed");
		return nullptr;
	}
	auto root = doc.child(L"����");
	if (root == nullptr)
		return nullptr;
	auto result = make_unique<ConfigLoader::BusConfig>();
	auto name = root.child(L"����");
	if (name)
	{
		result->name = StrConvert::wstringToUTF8(name.text().as_string());
	}
	auto xmlPub = root.child(L"����");
	if (xmlPub)
	{
		result->pubConfig = loadLinkCfg(xmlPub);
	}

	auto xmlSub = root.child(L"����");
	if (xmlSub)
	{
		result->subConfig = loadLinkCfg(xmlSub);
	}
	return result;
}

ZeromqLinkCfg ConfigLoader::loadLinkCfg(pugi::xml_node xmlNode)
{
	ZeromqLinkCfg cfg;

	auto xmlName = xmlNode.child(L"����");
	if (xmlName)
	{
		cfg.set_link_name(StrConvert::wstringToUTF8(xmlName.text().as_string()));
	}

	auto xmlAddress = xmlNode.child(L"��ַ");
	if (xmlAddress)
	{
		auto address = cfg.mutable_address();
		auto u8Ip = StrConvert::wstringToUTF8(xmlAddress.child(L"ip").text().as_string());
		address->set_ip(u8Ip);
		address->set_port(xmlAddress.child(L"�˿�").text().as_uint());
	}
	auto xmlHeartbeat = xmlNode.child(L"����");
	if (xmlHeartbeat)
	{
		auto heartbeat = cfg.mutable_heartbeat_cfg();
		heartbeat->set_heartbeat_interval(xmlHeartbeat.child(L"���").text().as_int());
		heartbeat->set_heartbeat_timeout(xmlHeartbeat.child(L"��ʱ").text().as_int());
		heartbeat->set_heartbeat_ttl(xmlHeartbeat.child(L"TTL").text().as_int());
	}
	auto xmlReconnection = xmlNode.child(L"����");
	if (xmlReconnection)
	{
		auto reconnection = cfg.mutable_reconnect_cfg();
		reconnection->set_reconnect_interval(xmlReconnection.child(L"���").text().as_int());
		reconnection->set_reconnect_interval_max(xmlReconnection.child(L"�����").text().as_int());
	}

	auto xmlSendOption = xmlNode.child(L"����");
	if (xmlSendOption)
	{
		auto sendOption = cfg.mutable_msg_send_cfg();
		sendOption->set_hwm(xmlSendOption.child(L"��ˮλ").text().as_int());
		sendOption->set_linger(xmlSendOption.child(L"����").text().as_int());
		sendOption->set_timeout(xmlSendOption.child(L"��ʱ").text().as_int());
	}
	return cfg;
}