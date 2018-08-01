#include "Rabbit.h"
#include <iostream>
#include <Windows.h>

#include <thread>
#include <mutex>
using namespace std;

#define producer_param std::string strIP = "127.0.0.1";\
int iPort = 5672;\
std::string strUser = "guest";\
std::string strPasswd = "guest";\
std::string strExchange = "test.exchangename"; \
std::string strExchangeType = "fanout"; \
std::string strRoutekey = "ALL.IP"; 


#define customer_param std::string strIP = "127.0.0.1";\
int iPort = 5672;\
std::string strUser = "guest";\
std::string strPasswd = "guest";\
std::string strExchange = "test.exchangename"; \
std::string strExchangeType = "fanout"; \


std::mutex mtx;


/*
* fanout 模式不处理路由键，只是简单的将队列绑定到交换机上
* 每个发送到交换机的消息都会被转发到与该交换机绑定的队列上
* 每个队列获取到了一份相同的数据
*/

void producer()
{
    producer_param;
    CRabbitmqClient mqClient;

    std::vector<std::string > vectQueueName;
    vectQueueName.push_back("Q1");
    vectQueueName.push_back("Q2");
    vectQueueName.push_back("Q3");
    vectQueueName.push_back("Q4");
    //广播只给定一个交换机名称即可？bindkey和routekey不用给
    std::vector<std::string > vectBindKey;
    vectBindKey.push_back("K1");
    vectBindKey.push_back("K2");
    vectBindKey.push_back("K3");
    vectBindKey.push_back("K4");

    //初始化MQ服务器
    if (0 == mqClient.InitMQ(strIP, iPort, strUser, strPasswd, strExchange, strExchangeType, vectQueueName, vectBindKey))
    {
        cout << "Rabbitmq Initlize Sucessed. " << endl;
    }
    else
    {
        cout << "Rabbitmq Initlize Failed. " << endl;
        return;
    }

    cout << "-----------------------------------------------------------------------------------------" << endl;
    cout << "模式：" << strExchangeType << ", 队列名：";
    for (auto qName: vectQueueName)
    {
        cout << qName << " | ";
    }
    cout << "\n-----------------------------------------------------------------------------------------" << endl;
    int index = 1, iRet = 0;

    while (true)
    {
        //发送消息
        std::string strSendMsg1 = "rabbitmq send test msg-" + std::to_string(index++);
        iRet = mqClient.publish(strSendMsg1);
        if (0 != iRet)
        {
            cout << "发送失败！！<"<<index <<">" << endl;
        }
        else
        {
            cout << "已经发送<" << index << ">条" << endl;
        }
        Sleep(400);
    }
}

void customer(int n)
{
    std::string strQueueName;
    switch (n)
    {
    case 0:strQueueName = "Q1"; break;
    case 1:strQueueName = "Q2"; break;
    case 2:strQueueName = "Q3"; break;
    case 3:strQueueName = "Q4"; break;
    default:break;
    }
    customer_param;
    CRabbitmqClient mqClient;

    std::vector<std::string > vectQueueName;
    std::vector<std::string > vectBindKey;

    //初始化MQ服务器
    if (0 == mqClient.InitMQ(strIP, iPort, strUser, strPasswd, strExchange, strExchangeType, vectQueueName, vectBindKey))
    {
        cout << "Rabbitmq Initlize Sucessed. " << endl;
    }
    else
    {
        cout << "Rabbitmq Initlize Failed. " << endl;
        return;
    }
    
    cout << "-----------------------------------------------------------------------------------------" << endl;
    cout << "模式：" << strExchangeType << ", 队列名：" << strQueueName << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;
    int index = 0;
    int iRet = 0;
    while (true)
    {
        cout << "-----------------------------------------------------------------------------------------" << endl;
        std::vector<std::string> vectMsg;
        //消费消息
        iRet = mqClient.consumer(strQueueName, vectMsg,1);
        if (0 != iRet)
        {
            cout << "队列：" << strQueueName <<" 消费消息失败！！<" << index << ">" << endl;
        }
        else
        {
            cout << "队列：" << strQueueName << "消费到第<"<< index++ << ">条消息，消息内容：：" << vectMsg.front()<< endl;
            std::map<int , string>
        }
        cout << "-----------------------------------------------------------------------------------------" << endl;
        Sleep(500);
    }
}
std::map<int, string>

//true 生产者 ； false 消费者
bool  bFlag = 0;

int main()
{
    if (bFlag)
    {
        std::thread tProducer(producer);
        tProducer.join();
    }
    else
    {
        std::thread tC1(customer, 0);
        //std::thread tC2(customer, 1);
        std::thread tC3(customer, 2);
        //std::thread tC4(customer, 3);

        tC1.detach();
        //tC2.detach();
        tC3.detach();
        //tC4.detach();
    }
    while (1)
        Sleep(100);
    system("pause");
    return 0;
}