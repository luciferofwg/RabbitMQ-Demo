#ifndef PRODUCER_H_CREATEDBYCHENFAN_ADAPTER_20130810_JSPTPD
#define PRODUCER_H_CREATEDBYCHENFAN_ADAPTER_20130810_JSPTPD

//#define IMEXPORTS
//
//#ifdef IMEXPORTS
//  #define  Publish __declspec(dllexport)
//#else
//  #define  Publish __declspec(dllimport)
//#endif


#include <string>
#include <vector>

using namespace std;


#include "amqp.h"
#include "MessageBody.h"


/**
*   @brief ��Ϣ���й�����
*
*   class CRabbitMQ in "RabbitMQ.h"
**/
class CRabbitMQ_Adapter
{

public:
	CRabbitMQ_Adapter(string HostName = "localhost", uint32_t port = 5672, string usr = "guest", string psw = "guest");

	~CRabbitMQ_Adapter();

	int32_t Connect(string &ErrorReturn = string(""));

	int32_t Disconnect(string &ErrorReturn = string(""));

	//����������exchange
	int32_t exchange_declare(CExchange &exchange, string &ErrorReturn = string(""));

	//������Ϣ����
	int32_t queue_declare(CQueue &queue, string &ErrorReturn = string(""));
	
	//�����У��������Ͱ󶨹���������γ�һ��·�ɱ�
	int32_t queue_bind(CQueue &queue, CExchange &exchange, const string bind_key, string &ErrorReturn = string(""));

	//�����У��������Ͱ󶨹���󶨽��
	int32_t queue_unbind(CQueue &queue, CExchange &exchange, const string bind_key, string &ErrorReturn = string(""));

	//������Ϣ
	int32_t publish(vector<CMessage> &message, string routkey, string &ErrorReturn = string(""));
	int32_t publish(CMessage &message, string routkey, string &ErrorReturn = string(""));
	int32_t publish(const string &message, string routkey, string &ErrorReturn = string(""));

	//������Ϣ
	int32_t consumer(CQueue &queue, vector<CMessage> &message, uint32_t GetNum = 1, struct timeval *timeout = NULL, string &ErrorReturn = string(""));
	int32_t consumer(const string & queue_name, vector<string> &message_array, uint32_t GetNum = 1000, struct timeval *timeout = NULL, string &ErrorReturn = string(""));


	// ɾ����Ϣ����
	int32_t queue_delete(const string queuename, int32_t if_unused = 0, string &ErrorReturn = string(""));


	// ��ö�����Ϣ����
	int32_t getMessageCount(const CQueue &queue, string &ErrorReturn = string(""));
	int32_t getMessageCount(const string &queuename, string &ErrorReturn = string(""));

	void setUser(const string UserName);

	string getUser() const;
	
	void setPassword(const string password);
	
	string getPassword() const;

	void __sleep(uint32_t millsecond);

private:
	// ȡ����Ϣ ȡ�ú�ɾ����Ϣʵ��
	int32_t read(const string QueueName, vector<string> &message, uint32_t GetNum = 1, struct timeval *timeout = NULL, string &ErrorReturn = string(""));

	// ����ͨ����
	void setChannel(const uint32_t channel);
	
	// ��õ�ǰͨ����
	uint32_t getChannel()const;
private:
	//����1�ɹ��������Ǵ���
	int32_t CRabbitMQ_Adapter::AssertError(amqp_rpc_reply_t x, string context, string &ErrorReturn);

	CRabbitMQ_Adapter(const CRabbitMQ_Adapter &other) //�������캯��
	{
	}
	CRabbitMQ_Adapter &operator=(const CRabbitMQ_Adapter &oter) //��ֵ����
	{
		return *this;
	}
private:
	string                  m_hostName;    //��Ϣ��������
	uint32_t                m_port;        //��Ϣ���ж˿�
	amqp_socket_t           *m_sock;
	amqp_connection_state_t m_conn;
	string					m_user;
	string					m_psw;
	uint32_t				m_channel;

	string m_routkey;
	CExchange *m_exchange;
	CQueue    *m_queue;
};


#endif

