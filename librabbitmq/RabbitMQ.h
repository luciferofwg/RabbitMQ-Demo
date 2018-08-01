#ifndef PRODUCER_H_CREATEDBYCHENFAN_20130724_JSPTPD
#define PRODUCER_H_CREATEDBYCHENFAN_20130724_JSPTPD

#include "SysDefine.h"
#include <string>
#include <vector>
#include "stdint.h"
#include "MessageBody.h"
#include "amqp.h"
using namespace std;

class CRabbitMQ_Adapter;

//��Ϣ���й�����
class AMQP_PUBLIC_FUNCTION CRabbitMQ
{
public:
	CRabbitMQ(string HostName = "localhost", uint32_t port = 5672, string usr = "guest", string psw = "guest");
	~CRabbitMQ();

	//������Ϣ���з�����
	int32_t Connect(string &ErrorReturn = string(""));

	//����Ϣ���з������Ͽ�����
	int32_t Disconnect(string &ErrorReturn = string(""));

	int32_t exchange_declare(CExchange &exchange, string &ErrorReturn = string(""));

	//������Ϣ����
	int32_t queue_declare(CQueue &queue, string &ErrorReturn = string(""));

	//�����У��������Ͱ󶨹���������γ�һ��·�ɱ�
	int32_t queue_bind(CQueue &queue, CExchange &exchange, const string bind_key, string &ErrorReturn = string(""));

	//�����У��������Ͱ󶨹���󶨽��
	int32_t queue_unbind(CQueue &queue, CExchange &exchange, const string bind_key, string &ErrorReturn = string(""));

	
	int32_t publish(vector<CMessage> &message, string routkey, string &ErrorReturn = string(""));

	int32_t publish(CMessage &message, string routkey, string &ErrorReturn = string(""));

	int32_t publish(const string &message, string routkey, string &ErrorReturn = string(""));

	int32_t consumer(CQueue &queue, vector<CMessage> &message, uint32_t GetNum = 1, struct timeval *timeout = NULL, string &ErrorReturn = string(""));
	int32_t consumer(const string & queue_name, vector<string> &message_array, uint32_t GetNum = 1000, struct timeval *timeout = NULL, string &ErrorReturn = string(""));


	int32_t queue_delete(const string queuename, int32_t if_unused = 0, string &ErrorReturn = string(""));

	//��ö�����Ϣ����
	int32_t getMessageCount(const CQueue &queue, string &ErrorReturn = string(""));
	int32_t getMessageCount(const string &queuename, string &ErrorReturn = string(""));

	void setUser(const string UserName);

	string getUser() const;

	void setPassword(const string password);

	string getPassword() const;

	void __sleep(uint32_t millsecond);
private:
	CRabbitMQ_Adapter *adapter;
};
#endif

