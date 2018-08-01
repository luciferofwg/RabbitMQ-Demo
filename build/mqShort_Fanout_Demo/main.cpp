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
* fanout ģʽ������·�ɼ���ֻ�Ǽ򵥵Ľ����а󶨵���������
* ÿ�����͵�����������Ϣ���ᱻת������ý������󶨵Ķ�����
* ÿ�����л�ȡ����һ����ͬ������
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
    //�㲥ֻ����һ�����������Ƽ��ɣ�bindkey��routekey���ø�
    std::vector<std::string > vectBindKey;
    vectBindKey.push_back("K1");
    vectBindKey.push_back("K2");
    vectBindKey.push_back("K3");
    vectBindKey.push_back("K4");

    //��ʼ��MQ������
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
    cout << "ģʽ��" << strExchangeType << ", ��������";
    for (auto qName: vectQueueName)
    {
        cout << qName << " | ";
    }
    cout << "\n-----------------------------------------------------------------------------------------" << endl;
    int index = 1, iRet = 0;

    while (true)
    {
        //������Ϣ
        std::string strSendMsg1 = "rabbitmq send test msg-" + std::to_string(index++);
        iRet = mqClient.publish(strSendMsg1);
        if (0 != iRet)
        {
            cout << "����ʧ�ܣ���<"<<index <<">" << endl;
        }
        else
        {
            cout << "�Ѿ�����<" << index << ">��" << endl;
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

    //��ʼ��MQ������
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
    cout << "ģʽ��" << strExchangeType << ", ��������" << strQueueName << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;
    int index = 0;
    int iRet = 0;
    while (true)
    {
        cout << "-----------------------------------------------------------------------------------------" << endl;
        std::vector<std::string> vectMsg;
        //������Ϣ
        iRet = mqClient.consumer(strQueueName, vectMsg,1);
        if (0 != iRet)
        {
            cout << "���У�" << strQueueName <<" ������Ϣʧ�ܣ���<" << index << ">" << endl;
        }
        else
        {
            cout << "���У�" << strQueueName << "���ѵ���<"<< index++ << ">����Ϣ����Ϣ���ݣ���" << vectMsg.front()<< endl;
            std::map<int , string>
        }
        cout << "-----------------------------------------------------------------------------------------" << endl;
        Sleep(500);
    }
}
std::map<int, string>

//true ������ �� false ������
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