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
    CRabbitmqClient mqClient;                                                //初始化MQ服务器
    int iRet = mqClient.InitMQ(strIP, iPort, strUser, strPasswd, strExchange, strExchangeType, strRoutekey, strQueuename);
    std::cout << "RabbitMQ 服务器连接成功" << std::endl;

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
            printf("Rabbitmq Publish failed, <index=%d>\n", nCount);
        }
        else
        {
            if (0 == nCount % 100)
            {
                std::cout << "生产消息[" << nCount << "] 条" << std::endl;
            }
            /*std::cout << "生产消息[" << strQueuename << "]成功,Msg:" << strSendMsg1 << std::endl;*/
        }
        Sleep(10);
    }
    //断开连接
    mqClient.ReleaseMQ();
}

void cunstomer()
{

}

int main()
{

    return 0;
}