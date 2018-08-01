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
    * @brief    ������Ϣ
    * @param    ��Ϣʵ��
    * @return   ����0ֵ����ɹ�������Ϣʵ�壬С��0�����ʹ���
    */
    int publish(const std::string& strMessage);
    /**
    * @brief    ������Ϣ
    * @param    ��������
    * @param    ��ȡ����Ϣʵ��
    * @param    ��Ҫȡ�õ���Ϣ����
    * @param    ȡ�õ���Ϣ���ӳ٣���ΪNULL����ʾ����ȡ�����ӳ٣�����״̬
    * @return   ����0ֵ����ɹ���С��0������󣬴�����Ϣ��ErrorReturn����
    */
    int consumer(const std::string &strQueueName, 
        std::vector<std::string> &msgVect, 
        int GetNum = 1, 
        struct timeval *timeout = NULL);

private:
    CRabbitmqClient(const CRabbitmqClient & rh);
    void operator=(const CRabbitmqClient & rh);

    std::string     m_strHostname;      // amqp����
    int             m_iPort;            // amqp�˿�
    std::string     m_strUser;
    std::string     m_strPasswd;  
    std::string     m_strExchange;
    std::string     m_strExchangeType;
    std::string     m_strRoutekey;
    std::string     m_strQueuename;
    std::string     m_strBindKey;
};

#endif
