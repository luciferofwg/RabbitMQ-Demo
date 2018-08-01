// MqDemo.cpp : �������̨Ӧ�ó������ڵ㡣
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
    CRabbitmqClient mqClient;                                                //��ʼ��MQ������
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
    *   ������Ϣ
    */
    int nCount = 0;
    while (true)
    {
        if (nCount > 0x7FFFFFFF)
        {
            nCount = 0;
        }
        //������Ϣ
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
                std::cout << "������Ϣ[" << nCount << "] ��" << std::endl;
            }*/
            std::cout << "[" << getTime()<< "] pulish [" << strQueuename << "] sucessed, Msg:" << strSendMsg1 << std::endl;
        }
        Sleep(10);
    }
    //�Ͽ�����
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
                std::cout << "��������Ͽ�����" << std::endl;
            else
                std::cout << "û�����ѵ�����" << std::endl;
        }
        else
        {
            nCount+= vecRecvMsg.size();
            std::cout << "������Ϣ[" << nCount << "] ��" << std::endl;

            /*if (0 == nCount%10)
            {
                std::cout << "������Ϣ[" << nCount << "] ��"  << std::endl;
            }*/
            /*for (size_t i = 0; i < vecRecvMsg.size(); i++)
            {
                std::cout << "������Ϣ[" << strQueuename << "]�ɹ�,Msg:" << vecRecvMsg[i] << std::endl;
            }*/
        }
        Sleep(1000);
    }
    //�Ͽ�����
    mqClient.ReleaseMQ();
}

// true ������ false ������

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

