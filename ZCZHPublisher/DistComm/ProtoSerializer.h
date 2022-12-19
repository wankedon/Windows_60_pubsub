/**
 * @file ProtoSerializer.h
 * @brief protobuf message ���л������л�
 * @author װ����ҵ������� ���� 
 * @version 0.1
 * @date 2020-09-08
 * 
 * @copyright Copyright (c) 2020  �й����ӿƼ����Ź�˾����ʮһ�о���
 * 
 */
#pragma once

/**
 * @brief protobuf message ���л���
 * 
 * @tparam T 
 */
template <class T>
class ProtoSerializer
{
public:
/**
 * @brief ��proto message �������л���
 * 
 * @param protoMessage 
 */
	ProtoSerializer(const T& protoMessage)
		:buffer(protoMessage.ByteSizeLong())
	{
		if (!buffer.empty())
		{
			protoMessage.SerializeToArray(buffer.data(), (int)buffer.size());
		}
		
	}

/**
 * @brief ���л�������ݣ�һ���ò���
 * 
 * @return std::pair<const byte*, size_t> ָ�����ݵ�ָ������ݴ�С
 */
	std::pair<const byte*, size_t> data() const
	{
		return std::pair<const byte*, size_t>{buffer.data(), buffer.size()};
	}

/**
 * @brief �Ѵ��л���������ӵ���Ϣβ��
 * 
 * @param msg ����ӵ���ϢĿ��
 * @return true ��ӳɹ�
 * @return false ���ʧ��
 */
	bool addToZMsg(zmsg_t* msg)
	{
		if (!buffer.empty())
		{
			zmsg_addmem(msg, buffer.data(), buffer.size());
			return true;
		}
		else
		{
			return false;
		}
		
	}

/**
 * @brief �Ѵ��л������ݲ��뵽��Ϣͷ��
 * 
 * @param msg ���������ϢĿ��
 * @return true ����ɹ�
 * @return false ����ʧ��
 */
	bool pushToZMsg(zmsg_t* msg)
	{
		if (!buffer.empty())
		{
			zmsg_pushmem(msg, buffer.data(), buffer.size());
			return true;
		}
		else
		{
			return false;
		}
	}
private:
	std::vector<byte> buffer;	///< ���protobuf message �Ļ�����
};

/**
 * @brief protobuf message �ķ����л���
 * 
 * @tparam T ����ԭ����Ϣ����
 */
template <class T>
class ProtoDeserializer
{
public:
	/**
	 * @brief ��������ָ��ʹ�С���������л����������л���ֻ���ã����������ݣ�ע�����ݵ���Ч��
	 * 
	 * @param data ָ�����ݵ�ָ��
	 * @param size ���ݴ�С
	 */
	ProtoDeserializer(const void* data, size_t size)
		:data(data),
		size(size)
	{

	}

	/**
	 * @brief ����czmq����Ϣ֡���������л���
	 * 
	 * @param f 
	 */
	ProtoDeserializer(zframe_t* f)
		:data(nullptr),
		size(0)
	{
		if (f != nullptr)
		{
			data = zframe_data(f);
			size = zframe_size(f);
		}
	}

	/**
	 * @brief �����л�
	 * 
	 * @param t �����������������ɹ������ֽ����ָ�Ϊ��Ϣ
	 * @return true �����ɹ�
	 * @return false ����ʧ��
	 */
	bool parse(T& t)
	{
		if (size == 0)
			return false;
		return t.ParseFromArray(data, (int)size);
	}
private:
	const void* data;	///< ָ�����ݵ�ָ��
	size_t size;	///< ���ݴ�С
};