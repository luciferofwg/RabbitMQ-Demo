#ifndef RABBITMQ_CLIENT_H_
#define RABBITMQ_CLIENT_H_

#include "RabbitMQ.h"

class CRabbitmqClient 
{
public:
    CRabbitmqClient();
    ~CRabbitmqClient();

    int InitMQ(
        const std::string &strHostname,
        int iPort, 
        const std::string &strUser,
        const std::string &strPasswd,
        const std::string &strExchange,
        const std::string &strExchangeType,
        const std::string &strRoutekey,
        const std::string &strQueuename,
        const std::string &strBindKey
        );

    int ReleaseMQ();
    /**
    * @brief    发布消息
    * @param    消息实体
    * @return   等于0值代表成功发送消息实体，小于0代表发送错误
    */
    int publish(const std::string& strMessage);
    /**
    * @brief    消费消息
    * @param    队列名称
    * @param    获取的消息实体
    * @param    需要取得的消息个数
    * @param    取得的消息是延迟，若为NULL，表示持续取，无延迟，阻塞状态
    * @return   等于0值代表成功，小于0代表错误，错误信息从ErrorReturn返回
    */
    int consumer(const std::string &strQueueName, 
        std::vector<std::string> &msgVect, 
        int GetNum = 1, 
        struct timeval *timeout = NULL);

private:
    CRabbitmqClient(const CRabbitmqClient & rh);
    void operator=(const CRabbitmqClient & rh);

    std::string     m_strHostname;      // amqp主机
    int             m_iPort;            // amqp端口
    std::string     m_strUser;
    std::string     m_strPasswd;  
    std::string     m_strExchange;
    std::string     m_strExchangeType;
    std::string     m_strRoutekey;
    std::string     m_strQueuename;
    std::string     m_strBindKey;
};

#endif
