#include "Rabbit.h"

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


void producer()
{
    producer_param;
    CRabbitmqClient mqClient;                                                //��ʼ��MQ������
    int iRet = mqClient.InitMQ(strIP, iPort, strUser, strPasswd, strExchange, strExchangeType, strRoutekey, strQueuename);
    std::cout << "RabbitMQ ���������ӳɹ�" << std::endl;

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
            printf("Rabbitmq Publish failed, <index=%d>\n", nCount);
        }
        else
        {
            if (0 == nCount % 100)
            {
                std::cout << "������Ϣ[" << nCount << "] ��" << std::endl;
            }
            /*std::cout << "������Ϣ[" << strQueuename << "]�ɹ�,Msg:" << strSendMsg1 << std::endl;*/
        }
        Sleep(10);
    }
    //�Ͽ�����
    mqClient.ReleaseMQ();
}

void cunstomer()
{

}

int main()
{

    return 0;
}