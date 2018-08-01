// MqDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include "Rabbit.h"
#include <chrono>
#include <ctime>

#include <Windows.h>

#define PARAM std::string strIP = "172.20.36.159";\
int iPort = 5672;\
std::string strUser = "guest";\
std::string strPasswd = "guest";\
std::string strExchange = "test.exchange.long";\
std::string strExchangeType = "direct";\
std::string strRoutekey = "route_key.long";\
std::string strQueuename = "test.queue_name.long";

std::string getTime()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
    return tmp;
}
void Produce()
{
    PARAM
    CRabbitmqClient mqClient;                                                //初始化MQ服务器
    int iRet = mqClient.InitMQ(strIP, iPort, strUser, strPasswd, strExchange, strExchangeType, strRoutekey, strQueuename);
    if (0 == iRet)
    {
        std::cout << "RabbitMQ connect to server sucessed." << std::endl;
    }
    else
    {
        std::cout << "RabbitMQ connect to server failed." << std::endl;
        return;
    }
    

    /*
    *   生产消息
    */
    int nCount = 0;
    while (true)
    {
        if (nCount > 0x7FFFFFFF)
        {
            nCount = 0;
        }
        //发送消息
        std::string strSendMsg1 = "rabbitmq send test msg-" + std::to_string(nCount++);
        iRet = mqClient.Publish(strSendMsg1);
        if (iRet != 0)
        {
            if(-4 != iRet)
                printf("Rabbitmq Publish failed, <index=%d>\n", nCount);
        }
        else
        {
            /*if (0 == nCount % 100)
            {
                std::cout << "生产消息[" << nCount << "] 条" << std::endl;
            }*/
            std::cout << "[" << getTime()<< "] pulish [" << strQueuename << "] sucessed, Msg:" << strSendMsg1 << std::endl;
        }
        Sleep(10);
    }
    //断开连接
    mqClient.ReleaseMQ();
}


void Consumer()
{
    PARAM
    CRabbitmqClient mqClient;

    int iRet = mqClient.InitMQ(strIP, iPort, strUser, strPasswd, strExchange, strExchangeType, strRoutekey, strQueuename);
    if (0 == iRet)
    {
        std::cout << "RabbitMQ connect to server sucessed." << std::endl;
    }
    else
    {
        std::cout << "RabbitMQ connect to server failed." << std::endl;
        return;
    }
    int nCount = 0;
    while (true)
    {
        if (nCount > 0x7FFFFFFF)
        {
            nCount = 0;
        }
        std::vector<std::string> vecRecvMsg;
        timeval tm;
        tm.tv_sec = 0;
        tm.tv_usec = 10;
        iRet = mqClient.Consumer(strQueuename, vecRecvMsg, 1, &tm);
        if (iRet < 0)
        {
            if (-4 == iRet)
                std::cout << "与服务器断开连接" << std::endl;
            else
                std::cout << "没有消费到数据" << std::endl;
        }
        else
        {
            nCount+= vecRecvMsg.size();
            std::cout << "消费消息[" << nCount << "] 条" << std::endl;

            /*if (0 == nCount%10)
            {
                std::cout << "消费消息[" << nCount << "] 条"  << std::endl;
            }*/
            /*for (size_t i = 0; i < vecRecvMsg.size(); i++)
            {
                std::cout << "消费消息[" << strQueuename << "]成功,Msg:" << vecRecvMsg[i] << std::endl;
            }*/
        }
        Sleep(1000);
    }
    //断开连接
    mqClient.ReleaseMQ();
}

// true 生产者 false 消费者

bool bFlag = 0;
int main()
{
    if (bFlag)
        Produce();
    else
        Consumer();

    system("pause");
    return 0;
}

