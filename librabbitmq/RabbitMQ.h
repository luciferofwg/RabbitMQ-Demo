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

//消息队列工具类
class AMQP_PUBLIC_FUNCTION CRabbitMQ
{
public:
	CRabbitMQ(string HostName = "localhost", uint32_t port = 5672, string usr = "guest", string psw = "guest");
	~CRabbitMQ();

	//连接消息队列服务器
	int32_t Connect(string &ErrorReturn = string(""));

	//与消息队列服务器断开连接
	int32_t Disconnect(string &ErrorReturn = string(""));

	int32_t exchange_declare(CExchange &exchange, string &ErrorReturn = string(""));

	//声明消息队列
	int32_t queue_declare(CQueue &queue, string &ErrorReturn = string(""));

	//将队列，交换机和绑定规则绑定起来形成一个路由表
	int32_t queue_bind(CQueue &queue, CExchange &exchange, const string bind_key, string &ErrorReturn = string(""));

	//将队列，交换机和绑定规则绑定解除
	int32_t queue_unbind(CQueue &queue, CExchange &exchange, const string bind_key, string &ErrorReturn = string(""));

	
	int32_t publish(vector<CMessage> &message, string routkey, string &ErrorReturn = string(""));

	int32_t publish(CMessage &message, string routkey, string &ErrorReturn = string(""));

	int32_t publish(const string &message, string routkey, string &ErrorReturn = string(""));

	int32_t consumer(CQueue &queue, vector<CMessage> &message, uint32_t GetNum = 1, struct timeval *timeout = NULL, string &ErrorReturn = string(""));
	int32_t consumer(const string & queue_name, vector<string> &message_array, uint32_t GetNum = 1000, struct timeval *timeout = NULL, string &ErrorReturn = string(""));


	int32_t queue_delete(const string queuename, int32_t if_unused = 0, string &ErrorReturn = string(""));

	//获得队列消息个数
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

