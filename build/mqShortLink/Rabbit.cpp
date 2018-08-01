#include "Rabbit.h"
#include <windows.h>
#include <iostream>

CRabbitmqClient::CRabbitmqClient()
    : m_strHostname("")
    , m_iPort(0)
    , m_strUser("")
    , m_strPasswd("")
{
}

CRabbitmqClient::~CRabbitmqClient() 
{
}
int CRabbitmqClient::InitMQ(
    const string &strHostname, 
    int iPort, 
    const string &strUser, 
    const string &strPasswd, 
    const std::string &strExchange,
    const std::string &strExchangeType,
    const std::string &strRoutekey,
    const std::string &strQueuename,
    const std::string &strBindKey)
{
    int nRet = 0;
    if (strHostname.empty() || 
        strUser.empty() || 
        strPasswd.empty() || 
        strExchange.empty() ||
        //strExchangeType.empty() ||
        strRoutekey.empty() ||
        strQueuename.empty() ||
        iPort < 0)
    {
        printf("初始化MQ失败，错误原因：参数不合法！\n");
        return -1;
    }

    m_strHostname = strHostname;
    m_iPort = iPort;
    m_strUser = strUser;
    m_strPasswd = strPasswd;

    m_strExchange = strExchange;
    m_strExchangeType = strExchangeType;
    m_strRoutekey = strRoutekey;
    m_strQueuename = strQueuename;
    m_strBindKey = strBindKey;
    return nRet;
}

int CRabbitmqClient::ReleaseMQ()
{
    return 0;
}

int CRabbitmqClient::publish(const string &strMessage)
{
    std::string strErr;
    //构造对象
    CRabbitMQ   qmClient(m_strHostname, m_iPort, m_strUser, m_strPasswd);
    //定义交换机对象
    CExchange   exChange(m_strExchange, true, m_strExchangeType);
    //定义队列对象
    CQueue      queue(m_strQueuename, true);

    //建立连接
    if (qmClient.Connect(strErr) < 0)
    {
        cout << "+++++++++++++++++++"<<strErr << endl;
        return -1;
    }

    //setp1  声明一个交换机
    if (qmClient.exchange_declare(exChange, strErr) < 0)
    {
        cout << strErr << endl;
        return -1;
    }
    //step2 声明一个队列
    if (qmClient.queue_declare(queue, strErr) < 0)
    {
        cout << strErr << endl;
        return -1;
    }
    //step3 将交换机，队列和direct的路由规则绑定 ,direct方式是完全匹配,可以按照队列名称绑定
    if (qmClient.queue_bind(queue, exChange, m_strBindKey, strErr) < 0)
    {
        cout << strErr << endl;
        return -1;
    }

    if (qmClient.publish(strMessage, m_strRoutekey, strErr) < 0)	//根据路由键（RoutingKey）发送消息到交换机（Exchange）
    {
        cout << strErr << endl;
    }
    qmClient.Disconnect();	//关闭链接（Connection）
    return 0;
}

int CRabbitmqClient::consumer(const string &strQueueName, vector<std::string> &message_array, int GetNum, struct timeval *timeout/* = NULL*/)
{
    std::string strErr;
    //定义Queue对象
    CQueue      queue(strQueueName, true);

    CRabbitMQ qmClient(m_strHostname, m_iPort, m_strUser, m_strPasswd);
    if (qmClient.Connect(strErr) < 0)
    {
        cout << "Connect failed, Error Msg:" << strErr << endl;
        return -1;
    }
    std::vector<CMessage> vecGetMsg;

    if (qmClient.consumer(queue, vecGetMsg, GetNum, timeout, strErr) < 0)
    {
        cout << "consumer failed, Error Msg:" << strErr << endl;
    }
    else
    {
        for (vector<CMessage>::iterator it = vecGetMsg.begin(); it != vecGetMsg.end(); ++it)
        {
            std::string str((*it).m_data);
            message_array.push_back(str);
        }
    }
    qmClient.Disconnect();

    return 0;
}
