// mqShortLink.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>

#include "Rabbit.h"
#include <string>
#include <iostream>

//direct ģʽ
/*
1.	һ���������ʹ��rabbitMQ�Դ���exchange����������exchange������Ϊ���ַ�������Ϊdefault exchange��
2.	����ģʽ�²���Ҫ��exchange�����κ�binding����
3.	��Ϣ����ʱ��Ҫһ��routekey�����Լ򵥵����ΪҪ���͵��Ķ�������
4.	���vHost�в�����RouteKey��ָ���Ķ������֣������Ϣ�ᱻ����
*/
#define PARAM std::string strIP = "127.0.0.1";\
int iPort = 5672;\
std::string strUser = "guest";\
std::string strPasswd = "guest";\
std::string strExchange = "exchange_direct"; \
std::string strExchangeType = "direct"; \
std::string strRoutekey = "route_direct"; \
std::string strQueuename = "queue_direct";\
std::string strBindKey = strRoutekey;
//std::string strBindKey = "bind_direct";


#define PARAM_127 std::string strIP = "172.20.36.159";\
int iPort = 5672;\
std::string strUser = "guest";\
std::string strPasswd = "guest";\
std::string strQueuename = "ZDRYYJ";


void Produce()
{
    PARAM

    CRabbitmqClient mqClient;
    //��ʼ��MQ������
    int iRet = mqClient.InitMQ(strIP, iPort, strUser, strPasswd, strExchange, strExchangeType, strRoutekey, strQueuename, strBindKey);
    cout << "Rabbitmq Initlize Ret:" << iRet << endl;

    /*
    *   ������Ϣ
    */
    cout << "-----------------------------------------------------------------------------------------" << endl;
    cout << "ģʽ��" << strExchangeType << ", ��������" << strQueuename << " RouteKey:" << strRoutekey << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;
    int index = 1;
    while (true)
    {
        //������Ϣ
        std::string strSendMsg1 = "rabbitmq send test msg-" + std::to_string(index++);
        iRet = mqClient.publish(strSendMsg1);
        if (iRet != 0)
        {
            cout << "***************************" << endl;
            cout << "������Ϣ[" << strQueuename << "]ʧ�ܣ�<index=" << index << ">:" << strSendMsg1 << endl;
            cout << "***************************" << endl;
        }
        else
        {
            cout << "������Ϣ[" << strQueuename  << "]�ɹ���<index=" << index << ">:" << strSendMsg1  <<endl;
        }
        Sleep(1000);
    }
}


void Consumer()
{
    PARAM_127

    CRabbitmqClient mqClient;

    //��ʼ��MQ������
    int iRet = mqClient.InitMQ(strIP, iPort, strUser, strPasswd, "", "", "", strQueuename, "");
    cout << "Rabbitmq Initlize Ret:" << iRet << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;
    cout <<"��������" << strQueuename << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;

    /*
    *   ������Ϣ
    */
    int index = 1;
    while (true)
    {
        index = (index++ > 0x7FFFFFFF) ? 1 : index;

        //������Ϣ
        std::vector<std::string> vectMsg;
        iRet = mqClient.consumer(strQueuename, vectMsg, 2);
        if (iRet != 0)
        {
            cout << "Rabbitmq consumer failed, <index=" << index << ">" << endl;
        }
        else
        {
            /*if (0 == index%100)
            {
                cout << "���ѵ���Ϣ�� [" << index << "]" << endl;
            }*/
            
            for (auto it : vectMsg)
            {
                cout << "������Ϣ[" << strQueuename << "]�ɹ���<" << index << ">:" << it << endl;
            }
        }
        Sleep(1000);
    }
    //�Ͽ�����
    mqClient.ReleaseMQ();
}
int main()
{
    Produce();

    //Consumer();
    return 0;
}

