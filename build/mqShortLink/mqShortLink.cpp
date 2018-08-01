// mqShortLink.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>

#include "Rabbit.h"
#include <string>
#include <iostream>

//direct 模式
/*
1.	一般情况可以使用rabbitMQ自带的exchange：””（该exchange的名字为空字符串，即为default exchange）
2.	这种模式下不需要将exchange进行任何binding操作
3.	消息传递时需要一个routekey，可以简单的理解为要发送到的队列名字
4.	如果vHost中不存在RouteKey中指定的队列名字，则该消息会被抛弃
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
    //初始化MQ服务器
    int iRet = mqClient.InitMQ(strIP, iPort, strUser, strPasswd, strExchange, strExchangeType, strRoutekey, strQueuename, strBindKey);
    cout << "Rabbitmq Initlize Ret:" << iRet << endl;

    /*
    *   生产消息
    */
    cout << "-----------------------------------------------------------------------------------------" << endl;
    cout << "模式：" << strExchangeType << ", 队列名：" << strQueuename << " RouteKey:" << strRoutekey << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;
    int index = 1;
    while (true)
    {
        //发送消息
        std::string strSendMsg1 = "rabbitmq send test msg-" + std::to_string(index++);
        iRet = mqClient.publish(strSendMsg1);
        if (iRet != 0)
        {
            cout << "***************************" << endl;
            cout << "生产消息[" << strQueuename << "]失败：<index=" << index << ">:" << strSendMsg1 << endl;
            cout << "***************************" << endl;
        }
        else
        {
            cout << "生产消息[" << strQueuename  << "]成功：<index=" << index << ">:" << strSendMsg1  <<endl;
        }
        Sleep(1000);
    }
}


void Consumer()
{
    PARAM_127

    CRabbitmqClient mqClient;

    //初始化MQ服务器
    int iRet = mqClient.InitMQ(strIP, iPort, strUser, strPasswd, "", "", "", strQueuename, "");
    cout << "Rabbitmq Initlize Ret:" << iRet << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;
    cout <<"队列名：" << strQueuename << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;

    /*
    *   消费消息
    */
    int index = 1;
    while (true)
    {
        index = (index++ > 0x7FFFFFFF) ? 1 : index;

        //接收消息
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
                cout << "消费到消息数 [" << index << "]" << endl;
            }*/
            
            for (auto it : vectMsg)
            {
                cout << "消费消息[" << strQueuename << "]成功：<" << index << ">:" << it << endl;
            }
        }
        Sleep(1000);
    }
    //断开连接
    mqClient.ReleaseMQ();
}
int main()
{
    Produce();

    //Consumer();
    return 0;
}

