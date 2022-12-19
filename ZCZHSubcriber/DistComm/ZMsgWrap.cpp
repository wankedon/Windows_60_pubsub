/**
 * @file ZMsgWrap.cpp
 * @brief zeromq��Ϣ���л�����
 * @author װ����ҵ������� ���� 
 * @version 0.1
 * @date 2021-07-13
 * 
 * @copyright Copyright (c) 2021  �й����ӿƼ����Ź�˾����ʮһ�о���
 * 
 */
#include "pch.h"
#include "ZMsgWrap.h"

using namespace std;

string extractString(zmsg_t* msg)
{
	string result;
	char* str = zmsg_popstr(msg);
	if (str)
	{
		result = str;
		zstr_free(&str);
	}
	return result;
}