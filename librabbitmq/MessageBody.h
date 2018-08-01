
#ifndef PRODUCER_H_CREATEDBYCHENFAN_MESSAGE_BODY_20130910_JSPTPD
#define PRODUCER_H_CREATEDBYCHENFAN_MESSAGE_BODY_20130910_JSPTPD

#include "SysDefine.h"

#include "stdint.h"
#include <vector>
#include <string>
using namespace std;

/**
*   @brief ��Ϣ���е���Ϣʵ��
*
*   class Message in "MessageBody.h"
**/
class Publish CMessage
{
public:
	CMessage(string data, int32_t durable = 2) :
		m_data(data), m_durable(durable)
	{
		this->m_type = "text/plain";
	}
	CMessage(string data, string routekey, int32_t durable = 2) :
		m_data(data), m_routekey(routekey), m_durable(durable)
	{
		this->m_type = "text/plain";
	}
	//�������캯��
	CMessage(const CMessage &other)
	{
		this->m_data = other.m_data;
		this->m_durable = other.m_durable;
		this->m_type = other.m_type;
		this->m_routekey = other.m_routekey;
	}
	//���� ��ֵ= �����
	CMessage operator=(const CMessage &other)
	{
		if (this == &other) return *this;
		this->m_data = other.m_data;
		this->m_durable = other.m_durable;
		this->m_type = other.m_type;
		this->m_routekey = other.m_routekey;
		return *this;
	}

	string  m_data;             //��Ϣ���ݣ�Ŀǰֻ֧���ı������Բ���string
	int32_t m_durable;          //��Ϣ�Ƿ�־û�; 1 ���־û�; 2 �־û�
	string  m_type;             //��Ϣ����  Ŀǰֻ֧���ı����� "text/plain"
	string  m_routekey;         //��Ϣ·�� Ĭ��������Ϣ��·�ɶ��� ��msg.����ͷ
								//size_t  length;           //��Ϣ����
								//void    *data;            //��Ϣ����
};


/**
*   @brief ��Ϣ���еĶ���ʵ��
*
*   class Message in "RabbitMQ.h"
**/
class Publish CQueue
{
public:
	CQueue(string name, int32_t durable = 1) :
		m_name(name), m_durable(durable)
	{
		m_auto_delete = 0;
		m_exclusive = 0;
		m_passive = 0;
	}
	//�������캯��
	CQueue(const CQueue &other)
	{
		this->m_name = other.m_name;
		this->m_durable = other.m_durable;
		this->m_auto_delete = other.m_auto_delete;
		this->m_exclusive = other.m_exclusive;
		this->m_passive = other.m_passive;
	}
	//���� ��ֵ= �����
	CQueue operator=(const CQueue &other)
	{
		if (this == &other) return *this;
		this->m_name = other.m_name;
		this->m_durable = other.m_durable;
		this->m_auto_delete = other.m_auto_delete;
		this->m_exclusive = other.m_exclusive;
		this->m_passive = other.m_passive;
		return *this;
	}
	string   m_name;         //��Ϣ��������
	int32_t  m_durable;      //��Ϣ�����Ƿ�־û�
	int32_t  m_exclusive;    //��ǰ���Ӳ���ʱ�������Ƿ��Զ�ɾ��
	int32_t  m_auto_delete;  //û��consumerʱ�������Ƿ��Զ�ɾ��
	int32_t  m_passive;
};

/**
*   @brief ��Ϣ���еĶ���ʵ��
*
*   class Message in "RabbitMQ.h"
**/
class Publish CExchange
{
public:
	CExchange(string name, int32_t durable = 1, string type = "direct") :
		m_name(name), m_durable(durable), m_type(type)
	{
		m_passive = 0;
	}
	//�������캯��
	CExchange(const CExchange &other)
	{
		this->m_name = other.m_name;
		this->m_durable = other.m_durable;
		this->m_type = other.m_type;
		this->m_passive = other.m_passive;
	}
	//���� ��ֵ= �����
	CExchange operator=(const CExchange &other)
	{
		if (this == &other) return *this;
		this->m_name = other.m_name;
		this->m_durable = other.m_durable;
		this->m_type = other.m_type;
		this->m_passive = other.m_passive;
		return *this;
	}
	string   m_name;         //����������
	string   m_type;         //����������  "fanout"  "direct" "topic"��ѡһ
	int32_t  m_durable;      //�������Ƿ�־û�
	int32_t  m_passive;

};


#endif